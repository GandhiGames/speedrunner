#pragma once

#include <memory>
#include <SFML\Graphics.hpp>

class SharedContext
{
	friend class Object;
	friend class Map;
	template<typename T> friend class ResourceManager;
public:
	sf::RenderWindow* m_window;
	std::shared_ptr<Object> m_player;
	ResourceManager<sf::Texture>* m_textureManager;
	Map* m_map;
};