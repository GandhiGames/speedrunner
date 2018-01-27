//
//  C_PlayAnimationOnDamage.hpp
//  speedrunner
//
//  Created by Robert Wells on 25/01/2018.
//  Copyright © 2018 Robert Wells. All rights reserved.
//

#ifndef C_PlayAnimationOnDamage_hpp
#define C_PlayAnimationOnDamage_hpp

#include "Component.h"
#include "C_Damageable.h"
#include "C_AnimatedSprite.h"

class C_PlayAnimationOnDamage : public Component, public C_Damageable
{
public:
    C_PlayAnimationOnDamage(Object* owner);
    
    void Awake() override;
    
    void SetAnimationOnDamage(ANIMATION_STATE gotToState);
    
    void DoDamage(const sf::Vector2f& dir, int damage) override;
    
private:
    ANIMATION_STATE m_stateToPlay;
    std::shared_ptr<C_AnimatedSprite> m_animationController;
};

#endif /* C_PlayAnimationOnDamage_hpp */
