//
//  MapBackgroundLayer.cpp
//  speedrunner
//
//  Created by Robert Wells on 26/01/2018.
//  Copyright © 2018 Robert Wells. All rights reserved.
//

#include "MapBackgroundLayer.hpp"

MapBackgroundLayer::MapBackgroundLayer(SharedContext& context) : m_context(context),
m_moveSpeed(1.f),
m_prevX(0)
{
}

void MapBackgroundLayer::Update(float targetX, float deltaTime)
{
    const float to = targetX - m_prevX;
    const float move = to  * m_moveSpeed;
    const sf::Vector2f& pos = m_sprite.getPosition();
    
    m_sprite.setPosition(pos.x - move, pos.y);
    
    m_prevX = targetX;
}

void MapBackgroundLayer::Draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}

void MapBackgroundLayer::SetSprite(int id)
{
    std::shared_ptr<sf::Texture> texture = m_context.m_textureManager->Get(id);
    
    texture->setRepeated(true);
    
    m_sprite.setTexture(*texture);
    
    //TODO: These are currently hard-coded but shouldn't be
    m_sprite.setPosition(-500.f, -40.f);
    m_sprite.setScale(1.5f, 1.5f);
    m_sprite.setTextureRect(sf::IntRect(0, 0, 2000, 307));
}

void MapBackgroundLayer::SetMoveSpeed(float speed)
{
    m_moveSpeed = speed;
}

