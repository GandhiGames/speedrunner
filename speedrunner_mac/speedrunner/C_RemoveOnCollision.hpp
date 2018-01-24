//
//  C_RemoveOnCollision.hpp
//  speedrunner
//
//  Created by Robert Wells on 24/01/2018.
//  Copyright © 2018 Robert Wells. All rights reserved.
//

#ifndef C_RemoveOnCollision_hpp
#define C_RemoveOnCollision_hpp

#include "Component.h"
#include "C_Collidable.h"

class C_RemoveOnCollision : public Component, public C_Collidable
{
public:
    C_RemoveOnCollision(Object* owner);
    ~C_RemoveOnCollision();
    
    void OnTriggerEnter(Object& owner, std::shared_ptr<Object> other) override;
};

#endif /* C_RemoveOnCollision_hpp */
