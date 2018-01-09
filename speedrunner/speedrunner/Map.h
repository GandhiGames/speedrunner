#pragma once

#include <unordered_map>
#include <memory>
#include <fstream>
#include <sstream>

#include "Tile.h"

// Holds the actual tile map.
using TileMap = std::unordered_map<unsigned int, std::shared_ptr<Tile>>;
// Holds all different tile types.
using TileSet = std::unordered_map<unsigned int, std::shared_ptr<TileInfo>>;

class Map
{
public:
	Map(SharedContext& context);
	~Map();

	std::shared_ptr<Tile> GetTile(unsigned int x, unsigned int y);
	TileInfo& GetDefaultTile();

	float GetGravity() const;
	unsigned int GetTileSize() const;
	const sf::Vector2u& GetMapSize() const;
	const sf::Vector2f& GetStartPosition() const;

	void LoadTiles(const std::string& tileDataPath, const std::string& tileTexturePath);
	void LoadMap(const std::string& mapFilePath);

	void Draw(sf::RenderWindow& window);

private:
	unsigned int ConvertTo1DCoord(unsigned int x, unsigned int y);
	void PurgeMap();
	void PurgeTileSet();

private:
	SharedContext& m_context;
	TileSet m_tileSet;
	TileMap m_map;
	TileInfo m_defaultTile;
	sf::Vector2u m_maxMapSize;
	sf::Vector2f m_startPosition;
	unsigned int m_tileCount;
	unsigned int m_tileSetCount;
	float m_gravity;
};

