#pragma once

#include "SharedContext.h"
#include "ResourceManager.h"
#include "Debug.h"


// Stores common tile data.
struct TileInfo
{
	TileInfo() :m_tileId(-1), m_textureId(-1)
	{
	}

	TileInfo(SharedContext& context, int textureId, unsigned int tileID, int x, int y, int width, int height)
	{
		ResourceManager<sf::Texture>* textureManager = context.m_textureManager;
		std::shared_ptr<sf::Texture> spriteTexture = textureManager->Get(textureId);

		if (!spriteTexture)
		{
			Debug::LogWarning("No texture found for Tile");
			return;
		}

		m_textureId = textureId;
		m_tileId = tileID;

		m_sprite.setTexture(*spriteTexture);

		sf::IntRect tileBoundaries(x * width, y * width, width, height);

		m_sprite.setTextureRect(tileBoundaries);
	}

	sf::Sprite m_sprite;
	int m_tileId;
	int m_textureId;
	//sf::Vector2f m_friction;
	//bool m_deadly;
};

struct Tile
{
	std::shared_ptr<TileInfo> m_properties;
};

