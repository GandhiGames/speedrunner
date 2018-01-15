#include "Map.h"
#include "TilemapParser.h"

Map::Map(SharedContext& context) : m_context(context),
m_defaultTile(),
m_mapParser(context)
{
	
}


Map::~Map()
{
}

TileInfo& Map::GetDefaultTile() { return m_defaultTile; }
const sf::Vector2u& Map::GetTileSize() const { return m_mapParser.GetTilesheetData()->m_tileSize; }
float Map::GetGravity() const { return m_mapParser.GetMapData()->m_gravity; }
const sf::Vector2u& Map::GetMapSize() const { return m_mapParser.GetMapData()->m_mapSize; }
const sf::Vector2f& Map::GetStartPosition() const { return m_mapParser.GetStartPosition(); }

void Map::LoadMap(std::string mapFilePath, std::string mapFileName)
{
	m_mapParser.Parse(mapFilePath, mapFileName);
}

std::shared_ptr<Tile> Map::GetTile(unsigned int x, unsigned int y)
{
	const sf::Vector2u& mapSize = GetMapSize(); //TODO: cache 
	TileMap& map = m_mapParser.GetMap();
	auto itr = map.find(Mathf::to1DIndex(x, y, mapSize.x));
	return(itr != map.end() ? itr->second : nullptr);
}

void Map::Draw(sf::RenderWindow& window)
{
	//TODO: create window class and move this to window.getviewspace
	sf::Vector2f viewCenter(window.getView().getCenter());
	sf::Vector2f viewSize(window.getView().getSize());
	sf::Vector2f viewSizeHalf(viewSize.x / 2, viewSize.y / 2);
	sf::FloatRect viewSpace(viewCenter - viewSizeHalf, viewSize);

	//TODO: cache this.
	const unsigned int tileSize = GetTileSize().x;

	// Set up culling space
	sf::Vector2i tileBegin(floor(viewSpace.left / tileSize), floor(viewSpace.top / tileSize));
	sf::Vector2i tileEnd(ceil((viewSpace.left + viewSpace.width) / tileSize), ceil((viewSpace.top + viewSpace.height) / tileSize));

	const unsigned int xMax = m_mapParser.GetMapData()->m_mapSize.x;
	const unsigned int yMax = m_mapParser.GetMapData()->m_mapSize.y;

	for (int x = tileBegin.x; x <= tileEnd.x; ++x)
	{
		for (int y = tileBegin.y; y <= tileEnd.y; ++y)
		{
			if (x < 0 || y < 0 ) { continue; }
			if (x > xMax || y > yMax) { break; }

			std::shared_ptr<Tile> tile = GetTile(x, y);

			if (!tile) { continue; }

			//TODO: do we need to set the sprite position every frame? Can we not set it once during initialisation.
			sf::Sprite& sprite = tile->m_properties->m_sprite;
			sprite.setPosition(x * tileSize, y * tileSize);
			window.draw(sprite);

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


