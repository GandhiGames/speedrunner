//
//  C_DamageOnCollision.hpp
//  speedrunner
//
//  Created by Robert Wells on 24/01/2018.
//  Copyright © 2018 Robert Wells. All rights reserved.
//

#ifndef C_DamageOnCollision_hpp
#define C_DamageOnCollision_hpp

#include <string>

#include "Component.h"
#include "C_Collidable.h"
#include "C_Tag.h"

class C_DamageOnCollision : public Component, public C_Collidable
{
public:
    C_DamageOnCollision(Object* owner);
    
    void OnTriggerEnter(Object& owner, std::shared_ptr<Object> other);
    
    void SetTargetTag(TAG target);
    
private:
    int m_dmgAmount;
    TAG m_targetTag;
};

#endif /* C_DamageOnCollision_hpp */
