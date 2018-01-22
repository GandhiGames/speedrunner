#pragma once

#include <unordered_map>
#include <memory>
#include <vector>
#include <sstream>
#include <algorithm>

#include "Debug.h"
#include "Tile.h"
#include "Util.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"

using namespace rapidxml;

struct TileSheetData
{
	int m_textureId;
	sf::Vector2u m_imageSize;
	int m_columns;
	int m_rows;
	sf::Vector2u m_tileSize;
	int m_tileCount;
};

struct MapData
{
	sf::Vector2u m_mapSize;
	int m_gravity;
};

struct TileMapSprite
{
	std::shared_ptr<TileSheetData> m_data;
	int m_tileSheetID;
};

using TileMap = std::unordered_map<unsigned int, std::shared_ptr<Tile>>; // Stores actual tile map.
using TileSet = std::unordered_map<unsigned int, std::shared_ptr<TileInfo>>; // Stores different tile types.

class TilemapParser
{
public:
	TilemapParser(SharedContext& context);
	~TilemapParser();

	void Parse(const std::string& fileLocation, const std::string& fileName);
	const std::shared_ptr<TileSheetData> GetTilesheetData() const;
	const std::shared_ptr<MapData> GetMapData() const;

	TileMap& GetMap();

	const sf::Vector2f& GetStartPosition() const;

	void PurgeMap();
	void PurgeTileSet();

private:
	void BuildTileSheetData(xml_node<>* rootNode);
	void BuildMapData(xml_node<>* rootNode);
	void BuildPlatformLayer(xml_node<>* playerformLayer);
	void SetPlayersPosition(xml_node<>* playerPositionLayer);

private:
	std::shared_ptr<TileSheetData> m_sheetData;
	std::shared_ptr<MapData> m_mapData;
	TileSet m_tileSet;
	TileMap m_map;
	SharedContext& m_context;
	std::string m_tileMapDataFileLocation;
	sf::Vector2f m_startPosition;
};

