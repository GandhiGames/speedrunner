//
//  C_DamageOnCollision.cpp
//  speedrunner
//
//  Created by Robert Wells on 24/01/2018.
//  Copyright © 2018 Robert Wells. All rights reserved.
//

#include "C_DamageOnCollision.hpp"
#include "Object.h"

C_DamageOnCollision::C_DamageOnCollision(Object* owner) : Component(owner),
m_dmgAmount(1),
m_targetTag(TAG::DEFAULT)
{
}

void C_DamageOnCollision::OnTriggerEnter(Object& owner, std::shared_ptr<Object> other)
{
    if(other->m_tag->Compare(m_targetTag))
    {
        auto damageables = other->GetComponents<C_Damageable>();
        
        for (auto& d : damageables)
        {
            const sf::Vector2f heading = other->m_transform->GetPosition() - owner.m_transform->GetPosition();
            const float dist = Mathf::magnitude(heading);
            const sf::Vector2f dir = heading / dist;
            
            d->DoDamage(dir, m_dmgAmount);
        }
    }
}

void C_DamageOnCollision::SetTargetTag(TAG target)
{
    m_targetTag = target;
}
