#pragma once

#include <memory>

#include "Component.h"
#include "C_Velocity.h"
#include "Util.h"

//TODO: add method for checking if direction has changed since last frame.ß
class C_Direction : public Component, public C_Updateable
{
public:
	C_Direction(Object* owner);

	void Awake() override;

	MOVEMENT_DIRECTION Get();
    
private:
	std::shared_ptr<C_Velocity> m_movement;
    MOVEMENT_DIRECTION m_moveDir;
};

