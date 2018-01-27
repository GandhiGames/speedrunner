#include "MapBackground.h"



MapBackground::MapBackground(SharedContext& context) :
m_context(context)
{
}


MapBackground::~MapBackground()
{
}

void MapBackground::Add(std::shared_ptr<MapBackgroundLayer> layer)
{
    m_layers.emplace_back(layer);
}

void MapBackground::Update(float targetPosition, float deltaTime)
{
    for(auto l : m_layers)
    {
        l->Update(targetPosition, deltaTime);
    }
}

void MapBackground::Draw(sf::RenderWindow& window)
{
    for (auto l : m_layers)
    {
        l->Draw(window);
    }
}
