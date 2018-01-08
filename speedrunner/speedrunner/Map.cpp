#include "Map.h"



Map::Map(SharedContext& context) : m_context(context),
m_defaultTile(context),
m_maxMapSize(32, 32), m_tileCount(0), m_tileSetCount(0),
m_gravity(512.f)
{
	
}


Map::~Map()
{
	PurgeMap();
	PurgeTileSet();
}


TileInfo& Map::GetDefaultTile() { return m_defaultTile; }
float Map::GetGravity() const { return m_gravity; }
unsigned int Map::GetTileSize() const { return Sheet::TILE_SIZE; }
const sf::Vector2u& Map::GetMapSize() const { return m_maxMapSize; }
const sf::Vector2f& Map::GetStartPosition() const { return m_startPosition; }

void Map::LoadMap(const std::string& mapFilePath)
{
	std::ifstream mapFile;
	mapFile.open(mapFilePath);
	if (!mapFile.is_open())
	{
		Debug::LogError("Cannot find map data file");
		return;
	}

	//EntityManager* entityMgr = m_context->m_entityManager;
	std::string line;

	while (std::getline(mapFile, line))
	{
		if (line[0] == '|') { continue; }

		std::stringstream keystream(line);
		std::string type;
		keystream >> type;

		if (type == "TILE")
		{
			int tileId = 0;
			keystream >> tileId;
			if (tileId < 0)
			{
				Debug::LogWarning("Bad tile id: " + tileId);
				continue;
			}

			auto itr = m_tileSet.find(tileId);
			if (itr == m_tileSet.end())
			{
				Debug::LogWarning("Tile id not found" + tileId);
				continue;
			}

			sf::Vector2i tileCoords;
			keystream >> tileCoords.x >> tileCoords.y;
			if (tileCoords.x > m_maxMapSize.x || tileCoords.y > m_maxMapSize.y)
			{
				Debug::LogWarning("Tile is out of range: " + tileCoords.x + ',' + tileCoords.y);
				continue;
			}

			std::shared_ptr<Tile> tile = std::make_shared<Tile>();
			// Bind properties of a tile from a set.
			tile->m_properties = itr->second;

			if (!m_map.emplace(ConvertTo1DCoord(tileCoords.x, tileCoords.y), tile).second)
			{
				// Duplicate tile detected!
				Debug::LogWarning("Duplicate tile: " + tileCoords.x + ',' + tileCoords.y);
				tile = nullptr;
				continue;
			}

			std::string warp;
			keystream >> warp;

			//TODO: do we want to re-implement warp tiles?
			//tile->m_warp = false;
			//if (warp == "WARP") { tile->m_warp = true; }
		}
		/*
		else if (type == "BACKGROUND") {
			if (m_backgroundTexture != "") { continue; }
			keystream >> m_backgroundTexture;
			if (!m_context->m_textureManager->RequireResource(m_backgroundTexture)) {
				m_backgroundTexture = "";
				continue;
			}
			sf::Texture* texture = m_context->m_textureManager->GetResource(m_backgroundTexture);
			m_background.setTexture(*texture);
			sf::Vector2f viewSize = m_currentState->GetView().getSize();
			sf::Vector2u textureSize = texture->getSize();
			sf::Vector2f scaleFactors;
			scaleFactors.x = viewSize.x / textureSize.x;
			scaleFactors.y = viewSize.y / textureSize.y;
			m_background.setScale(scaleFactors);
		}
		*/
		else if (type == "SIZE") 
		{
			keystream >> m_maxMapSize.x >> m_maxMapSize.y;
		}
		else if (type == "GRAVITY") 
		{
			keystream >> m_gravity;
		}
		else if (type == "DEFAULT_FRICTION") 
		{
			keystream >> m_defaultTile.m_friction.x >> m_defaultTile.m_friction.y;
		}
		/*
		else if (type == "NEXTMAP") {
			keystream >> m_nextMap;
		}
		*/
		else if (type == "PLAYER") 
		{
			float playerX = 0; float playerY = 0;
			keystream >> playerX >> playerY;
			m_startPosition = sf::Vector2f(playerX, playerY);
		}
		/*
		else if (type == "ENEMY") {
			std::string enemyName;
			keystream >> enemyName;
			int enemyId = entityMgr->Add(EntityType::Enemy, enemyName);
			if (enemyId < 0) { continue; }
			float enemyX = 0; float enemyY = 0;
			keystream >> enemyX >> enemyY;
			entityMgr->Find(enemyId)->SetPosition(enemyX, enemyY);
		}
		*/
		else 
		{
			Debug::LogWarning("Unknown map data type " + type);
		}
	}
	mapFile.close();
}

void Map::LoadTiles(const std::string& tileDataPath,
	const std::string& tileTexturePath)
{
	std::ifstream file;
	file.open(tileDataPath);

	if (!file.is_open())
	{
		Debug::LogWarning("Failed loading tile set file: " + tileDataPath);
		return;
	}

	int textureId = m_context.m_textureManager->Add(tileTexturePath);
	if (textureId < 0)
	{
		Debug::LogError("Map spritesheet not found");
		return;
	}

	std::string line;
	while (std::getline(file, line))
	{
		if (line[0] == '|') { continue; }

		std::stringstream keystream(line);

		int tileId;
		keystream >> tileId;
		if (tileId < 0) { continue; }

		std::shared_ptr<TileInfo> tile = std::make_shared<TileInfo>(m_context, textureId, tileId);
		keystream >> tile->m_name >> tile->m_friction.x >> tile->m_friction.y >> tile->m_deadly;

		if (!m_tileSet.emplace(tileId, tile).second)
		{
			Debug::LogWarning("Duplicate tile type: " + tile->m_name);
		}
	}
	file.close();
}

std::shared_ptr<Tile> Map::GetTile(unsigned int x, unsigned int y)
{
	auto itr = m_map.find(ConvertTo1DCoord(x, y));
	return(itr != m_map.end() ? itr->second : nullptr);
}

unsigned int Map::ConvertTo1DCoord(unsigned int x, unsigned int y)
{
	return (x * m_maxMapSize.x) + y;
}

void Map::Draw(sf::RenderWindow& window)
{
	//TODO: create window class and move this to window.getviewspace
	sf::Vector2f viewCenter(window.getView().getCenter());
	sf::Vector2f viewSize(window.getView().getSize());
	sf::Vector2f viewSizeHalf(viewSize.x / 2, viewSize.y / 2);
	sf::FloatRect viewSpace(viewCenter - viewSizeHalf, viewSize);

	// Set up culling space
	sf::Vector2i tileBegin(floor(viewSpace.left / Sheet::TILE_SIZE), floor(viewSpace.top / Sheet::TILE_SIZE));
	sf::Vector2i tileEnd(ceil((viewSpace.left + viewSpace.width) / Sheet::TILE_SIZE), ceil((viewSpace.top + viewSpace.height) / Sheet::TILE_SIZE));

	unsigned int count = 0;
	for (int x = tileBegin.x; x <= tileEnd.x; ++x)
	{
		for (int y = tileBegin.y; y <= tileEnd.y; ++y)
		{
			if (x < 0 || y < 0) { continue; }

			std::shared_ptr<Tile> tile = GetTile(x, y);

			if (!tile) { continue; }

			//TODO: do we need to set the sprite position every frame? Can we not set it once during initialisation.
			sf::Sprite& sprite = tile->m_properties->m_sprite;
			sprite.setPosition(x * Sheet::TILE_SIZE, y * Sheet::TILE_SIZE);
			window.draw(sprite);

			count++;

			// TODO: re-implement map debug drawing in debug class.
			/*
			if (m_context->m_debugOverlay.Debug()) {
				if (tile->m_properties->m_deadly || tile->m_warp) {
					sf::RectangleShape* tileMarker = new sf::RectangleShape(
						sf::Vector2f(Sheet::Tile_Size, Sheet::Tile_Size));
					tileMarker->setPosition(x * Sheet::Tile_Size, y * Sheet::Tile_Size);
					if (tile->m_properties->m_deadly) {
						tileMarker->setFillColor(sf::Color(255, 0, 0, 100));
					}
					else if (tile->m_warp) {
						tileMarker->setFillColor(sf::Color(0, 255, 0, 150));
					}
					m_context->m_debugOverlay.Add(tileMarker);
				}
			}
			*/
		}
	}
}

void Map::PurgeMap()
{
	m_tileCount = 0;
	m_map.clear();
}

void Map::PurgeTileSet()
{
	m_tileSetCount = 0;
	m_tileSet.clear();
}
