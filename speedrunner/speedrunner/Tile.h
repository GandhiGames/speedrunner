#pragma once

#include "SharedContext.h"
#include "ResourceManager.h"

//TODO: load maps from tmx

enum Sheet { TILE_SIZE = 32, WIDTH = 256, HEIGHT = 256 };

// Stores common tile data.
struct TileInfo
{
	TileInfo(SharedContext& context, int texture = -1, unsigned int tileID = 0)
	{
		ResourceManager<sf::Texture>* textureManager = context.m_textureManager;

		if (texture == -1)
		{
			m_tileId = tileID;
			return;
		}



	}

private:
	int m_tileId;
	int m_textureId;
};

class Tile
{
public:
	Tile();
	~Tile();
};

