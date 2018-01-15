#include "TilemapParser.h"
#include "ResourceManager.h"

TilemapParser::TilemapParser(SharedContext& context) : m_context(context), 
m_startPosition(0.f, 0.f), m_tileMapDataFileLocation("")
{
}


TilemapParser::~TilemapParser()
{

}

void TilemapParser::Parse(const std::string& fileLocation, const std::string& fileName)
{
	m_tileMapDataFileLocation = fileLocation;

	std::string file = fileLocation + fileName;
	char* cstr = new char[file.size() + 1];  
	strcpy_s(cstr, file.size() + 1, file.c_str());

	//TODO: error checking - check file exists before attempting open
	rapidxml::file<> xmlFile(cstr);
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());

	//TODO: add ability to pass string formats to debug
	//printf("Doc first name: %s", doc.first_node()->name->value());
	xml_node<>* rootNode = doc.first_node("map");

	BuildTileSheetData(rootNode);
	BuildMapData(rootNode);

}

//TODO:add support for mroe than one tilemap (including tilemaps of different sizes).
void TilemapParser::BuildTileSheetData(xml_node<>* rootNode)
{
	m_sheetData = std::make_shared<TileSheetData>();

	xml_node<>* tilesheetNode = rootNode->first_node("tileset");

	//TODO: add error checking to ensure these values actually exist.
	int firstid = std::atoi(tilesheetNode->first_attribute("firstgid")->value()); //TODO: implement this.
	m_sheetData->m_tileSize.x = std::atoi(tilesheetNode->first_attribute("tilewidth")->value());
	m_sheetData->m_tileSize.y = std::atoi(tilesheetNode->first_attribute("tileheight")->value());
	m_sheetData->m_tileCount = std::atoi(tilesheetNode->first_attribute("tilecount")->value());
	m_sheetData->m_columns = std::atoi(tilesheetNode->first_attribute("columns")->value());
	m_sheetData->m_rows = m_sheetData->m_tileCount / m_sheetData->m_columns;

	xml_node<>* imageNode = tilesheetNode->first_node("image");
	m_sheetData->m_textureId = m_context.m_textureManager->Add(m_tileMapDataFileLocation + std::string(imageNode->first_attribute("source")->value()));

	if (m_sheetData->m_textureId == -1)
	{
		Debug::LogError("Texture not found");
	}

	m_sheetData->m_imageSize.x = std::atoi(imageNode->first_attribute("width")->value());
	m_sheetData->m_imageSize.y = std::atoi(imageNode->first_attribute("height")->value());
}

void TilemapParser::BuildMapData(xml_node<>* rootNode)
{
	xml_node<>* layerNode = rootNode->first_node("layer");

	const std::string platformLayerName = "platform";
	const std::string playerStartLayerName = "player_start";

	std::unordered_map<std::string, xml_node<>*> platformLayers;
	for (xml_node<> * node = rootNode->first_node("layer"); node; node = node->next_sibling())
	{
		const char* layerName = node->first_attribute("name")->value();
		platformLayers.emplace(layerName, node);
	}

	auto platformIt = platformLayers.find(platformLayerName);
	if (platformIt != platformLayers.end())
	{
		BuildPlatformLayer(platformIt->second);
	}

	auto playerPosIt = platformLayers.find(playerStartLayerName);
	if (playerPosIt != platformLayers.end())
	{
		SetPlayersPosition(playerPosIt->second);
	}
}

void TilemapParser::BuildPlatformLayer(xml_node<>* platformLayer)
{
	m_mapData = std::make_shared<MapData>();
	m_mapData->m_mapSize.x = std::atoi(platformLayer->first_attribute("width")->value());
	m_mapData->m_mapSize.y = std::atoi(platformLayer->first_attribute("height")->value());
	m_mapData->m_gravity = 980; //TODO: load this from file.

	xml_node<>* dataNode = platformLayer->first_node("data");

	char* map = dataNode->value();

	std::stringstream fileStream(map);

	int count = 0;

	std::string line;
	while (fileStream.good())
	{
		std::string substr;
		std::getline(fileStream, substr, ',');

		if (!Util::IsInteger(substr))
		{
			substr.erase(std::remove(substr.begin(), substr.end(), '\r'), substr.end());
			substr.erase(std::remove(substr.begin(), substr.end(), '\n'), substr.end());

			if (!Util::IsInteger(substr))
			{
				Debug::LogError("Not int: " + substr);
			}
		}

		int tileId = std::stoi(substr);

		if (tileId != 0)
		{
			auto itr = m_tileSet.find(tileId);
			if (itr == m_tileSet.end())
			{
				int textureX = tileId % m_sheetData->m_columns - 1;
				int textureY = tileId / m_sheetData->m_columns;

				std::shared_ptr<TileInfo> tileInfo =
					std::make_shared<TileInfo>(
						m_context, m_sheetData->m_textureId, tileId,
						textureX, textureY,
						m_sheetData->m_tileSize.x, m_sheetData->m_tileSize.y);

				m_tileSet.emplace(tileId, tileInfo);

				itr = m_tileSet.find(tileId); //TODO: can we return an iterator from a insert/emplace method.
			}

			std::shared_ptr<Tile> tile = std::make_shared<Tile>();
			// Bind properties of a tile from a set.
			tile->m_properties = itr->second;

			int tileX = count % m_mapData->m_mapSize.x;
			int tileY = count / m_mapData->m_mapSize.x;

			if (!m_map.emplace(Mathf::to1DIndex(tileX, tileY, m_mapData->m_mapSize.x), tile).second)
			{
				// Duplicate tile detected!
				Debug::LogWarning("Duplicate tile: " + tileX + ',' + tileY);
			}
		}

		count++;
	}
}

void TilemapParser::SetPlayersPosition(xml_node<>* playerPositionLayer)
{
	//TODO: most of the below lines are repeated in BuildPlatformLayer, remove repition
	xml_node<>* dataNode = playerPositionLayer->first_node("data");
	char* map = dataNode->value();
	std::stringstream fileStream(map);
	int count = 0;
	std::string line;
	while (fileStream.good())
	{
		std::string substr;
		std::getline(fileStream, substr, ',');

		if (!Util::IsInteger(substr))
		{
			substr.erase(std::remove(substr.begin(), substr.end(), '\r'), substr.end());
			substr.erase(std::remove(substr.begin(), substr.end(), '\n'), substr.end());

			if (!Util::IsInteger(substr))
			{
				Debug::LogError("Not int: " + substr);
			}
		}

		int tileId = std::stoi(substr);

		if (tileId != 0)
		{
			m_startPosition.x = (count % m_mapData->m_mapSize.x) * m_sheetData->m_tileSize.x;
			m_startPosition.y = (count / m_mapData->m_mapSize.x) * m_sheetData->m_tileSize.y;

			return;
		}

		count++;
	}
}

const std::shared_ptr<TileSheetData> TilemapParser::GetTilesheetData() const
{
	return m_sheetData;
}

const std::shared_ptr<MapData> TilemapParser::GetMapData() const
{
	return m_mapData;
}

TileMap& TilemapParser::GetMap()
{
	return m_map;
}

void TilemapParser::PurgeMap()
{
	m_map.clear();
}

void TilemapParser::PurgeTileSet()
{
	m_tileSet.clear();
}

const sf::Vector2f& TilemapParser::GetStartPosition() const
{
	return m_startPosition;
}
