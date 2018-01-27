//
//  C_DirectionalAnimation.hpp
//  speedrunner
//
//  Created by Robert Wells on 25/01/2018.
//  Copyright © 2018 Robert Wells. All rights reserved.
//

#ifndef C_DirectionalAnimation_hpp
#define C_DirectionalAnimation_hpp

#include "Component.h"
#include "C_Updateable.h"
#include "C_AnimatedSprite.h"
#include "C_Direction.h"

//TODO: standardise naming convention for this vs C_MovementAnimation. Maybe this should be called C_DirectionalSprite
class C_DirectionalAnimation : public Component, public C_Updateable
{
public:
    C_DirectionalAnimation(Object* owner);
    
    void Awake() override;
    
    void LateUpdate(float deltaTime) override;
    
private:
    std::shared_ptr<C_AnimatedSprite> m_animation;
    std::shared_ptr<C_Direction> m_direction;
};

#endif /* C_DirectionalAnimation_hpp */
