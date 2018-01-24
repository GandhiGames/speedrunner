//
//  C_RemoveOnCollision.cpp
//  speedrunner
//
//  Created by Robert Wells on 24/01/2018.
//  Copyright © 2018 Robert Wells. All rights reserved.
//

#include "C_RemoveOnCollision.hpp"
#include "Object.h"

C_RemoveOnCollision::C_RemoveOnCollision(Object* owner) : Component(owner)
{
    
}

C_RemoveOnCollision::~C_RemoveOnCollision() {}

void C_RemoveOnCollision::OnTriggerEnter(Object& owner, std::shared_ptr<Object> other)
{
    owner.Destroy();
}
