//
//  C_PlayAnimationOnDamage.cpp
//  speedrunner
//
//  Created by Robert Wells on 25/01/2018.
//  Copyright © 2018 Robert Wells. All rights reserved.
//

#include "C_PlayAnimationOnDamage.hpp"
#include "Object.h"

C_PlayAnimationOnDamage::C_PlayAnimationOnDamage(Object* owner) : Component(owner),
m_stateToPlay(ANIMATION_STATE::HURT)
{
}

void C_PlayAnimationOnDamage::Awake()
{
    m_animationController = m_owner->GetComponent<C_AnimatedSprite>();
}

void C_PlayAnimationOnDamage::SetAnimationOnDamage(ANIMATION_STATE gotToState)
{
    m_stateToPlay = gotToState;
}

void C_PlayAnimationOnDamage::DoDamage(const sf::Vector2f& dir, int damage)
{
    m_animationController->SetAnimationState(m_stateToPlay);
}

