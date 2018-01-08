#pragma once

#include <memory>
#include <SFML\Graphics.hpp>

#include "Object.h"
#include "ResourceManager.h"

class SharedContext
{
public:
	sf::RenderWindow* m_window;
	std::shared_ptr<Object> m_player;
	ResourceManager<sf::Texture>* m_textureManager;
};