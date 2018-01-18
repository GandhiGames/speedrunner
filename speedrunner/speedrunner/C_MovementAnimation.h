#pragma once

#include<tuple>
#include<array>

#include "Component.h"
#include "C_Updateable.h"
#include "C_AnimatedSprite.h"
#include "C_Velocity.h"
#include "C_Direction.h"
#include "C_MapCollision.h"
#include "Util.h"

//TODO: rename class
class C_MovementAnimation : public Component, public C_Updateable
{
	friend class Object;

public:
	C_MovementAnimation(Object* owner);

	void Awake() override;

	void Update(float timeDelta) override;

private:
	std::shared_ptr<C_AnimatedSprite> m_sprite;
	std::shared_ptr<C_Velocity> m_movement;
	std::shared_ptr<C_MapCollision> m_mapCollision;

	float m_minWalkVelocity;
	float m_minJumpVelocity;
};

