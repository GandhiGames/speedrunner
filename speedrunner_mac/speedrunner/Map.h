#pragma once

#include <unordered_map>
#include <memory>
#include <fstream>
#include <sstream>

#include "Tile.h"
#include "Util.h"
#include "TilemapParser.h"
#include "MapBackground.h"

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
    void LoadMap(const std::string& mapFilePath, const std::string& mapFileName);

    //TODO: what do we do if there are variable number of background layers?
    void LoadBackground(std::vector<std::string>& layerPaths);
    
    void Update(float target, float deltaTime);
	void Draw(sf::RenderWindow& window);

private:
	SharedContext& m_context;
	TilemapParser m_mapParser; 
	TileInfo m_defaultTile;
    MapBackground m_background;
};

