#pragma once

#include <SFML/Graphics.hpp>

#include "SharedContext.h"
#include "ResourceManager.h"
#include "MapBackgroundLayer.hpp"

class MapBackground
{
public:
	MapBackground(SharedContext& context);
	~MapBackground();

    //TODO: lookup the negatives of using shared_ptrs
    void Add(std::shared_ptr<MapBackgroundLayer> layer);
    
    void Update(float targetPosition, float deltaTime);
    void Draw(sf::RenderWindow& window);
    
    
private:
    SharedContext& m_context;
    std::vector<std::shared_ptr<MapBackgroundLayer>> m_layers;
};

