//
//  C_DirectionalAnimation.cpp
//  speedrunner
//
//  Created by Robert Wells on 25/01/2018.
//  Copyright © 2018 Robert Wells. All rights reserved.
//

#include "C_DirectionalAnimation.hpp"
#include "Object.h"

C_DirectionalAnimation::C_DirectionalAnimation(Object* owner) : Component(owner)
{
}

void C_DirectionalAnimation::Awake()
{
    m_animation = m_owner->GetComponent<C_AnimatedSprite>();
    m_direction = m_owner->GetComponent<C_Direction>();
    
}

void C_DirectionalAnimation::LateUpdate(float deltaTime)
{
    m_animation->SetDirection(m_direction->Get());
}

