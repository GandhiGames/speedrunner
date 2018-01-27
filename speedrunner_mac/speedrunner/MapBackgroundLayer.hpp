//
//  MapBackgroundLayer.hpp
//  speedrunner
//
//  Created by Robert Wells on 26/01/2018.
//  Copyright © 2018 Robert Wells. All rights reserved.
//

#ifndef MapBackgroundLayer_hpp
#define MapBackgroundLayer_hpp

#include <SFML/Graphics.hpp>
#include <math.h>

#include "SharedContext.h"
#include "ResourceManager.h"

class MapBackgroundLayer
{
public:
    MapBackgroundLayer(SharedContext& context);
    void Update(float targetX, float deltaTime);
    void Draw(sf::RenderWindow& window);
    void SetMoveSpeed(float speed);
    void SetSprite(int id);
    void Load();
    
private:
    SharedContext& m_context;
    sf::Sprite m_sprite;
    float m_moveSpeed;
    float m_prevX;
};

#endif /* MapBackgroundLayer_hpp */
