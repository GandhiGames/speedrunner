#pragma once

#include "SharedContext.h"
#include "ResourceManager.h"

#include "Debug.h"

//TODO: load maps from tmx

enum Sheet { TILE_SIZE = 32, WIDTH = 256, HEIGHT = 256 };

// Stores common tile data.
struct TileInfo
{
	TileInfo(SharedContext& context, int textureId = -1, unsigned int tileID = 0)
	{
		ResourceManager<sf::Texture>* textureManager = context.m_textureManager;

		if (textureId == -1)
		{
			m_tileId = tileID;
			return;
		}

		std::shared_ptr<sf::Texture> spriteTexture = textureManager->Get(textureId);

		if (!spriteTexture)
		{
			Debug::LogWarning("No texture found for Tile");
			return;
		}

		m_textureId = textureId;
		m_tileId = tileID;

		m_sprite.setTexture(*spriteTexture);

		sf::IntRect tileBoundaries
		(
			m_tileId % (Sheet::WIDTH / Sheet::TILE_SIZE) * Sheet::TILE_SIZE,
			m_tileId / (Sheet::HEIGHT / Sheet::TILE_SIZE) * Sheet::TILE_SIZE,
			Sheet::TILE_SIZE, Sheet::TILE_SIZE
		);

		m_sprite.setTextureRect(tileBoundaries);
	}

	std::string m_name;
	sf::Sprite m_sprite;
	int m_tileId;
	int m_textureId;
	sf::Vector2f m_friction;
	bool m_deadly;
};

struct Tile
{
	std::shared_ptr<TileInfo> m_properties;
};

