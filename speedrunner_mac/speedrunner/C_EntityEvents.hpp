//
//  C_EntityEvents.hpp
//  speedrunner
//
//  Created by Robert Wells on 30/01/2018.
//  Copyright © 2018 Robert Wells. All rights reserved.
//

#ifndef C_EntityEvents_hpp
#define C_EntityEvents_hpp

#include "Component.h"
#include "EventSystem.hpp"

class C_EntityEvents : public Component
{
public:
    
    C_EntityEvents(Object* owner) : Component(owner){}
    
    
    EventSystem m_eventSystem;
};

#endif /* C_EntityEvents_hpp */
