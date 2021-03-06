#pragma once

#include <unordered_map>
#include <memory>
#include <fstream>
#include <sstream>

#include "Tile.h"
#include "Util.h"
#include "TilemapParser.h"


class Map
{
public:
	Map(SharedContext& context);
	~Map();

	std::shared_ptr<Tile> GetTile(unsigned int x, unsigned int y);
	TileInfo& GetDefaultTile();

	float GetGravity() const;
	const sf::Vector2u& GetTileSize() const;
	const sf::Vector2u& GetMapSize() const;
	const sf::Vector2f& GetStartPosition() const;

	void LoadTiles(const std::string& tileDataPath, const std::string& tileTexturePath);
	void LoadMap(std::string mapFilePath, std::string mapFileName);

	void Draw(sf::RenderWindow& window);

private:
	SharedContext& m_context;
	TilemapParser m_mapParser; 
	TileInfo m_defaultTile;
};

