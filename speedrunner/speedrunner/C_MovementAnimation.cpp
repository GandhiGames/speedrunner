#include "C_MovementAnimation.h"

#include "C_MovementAnimation.h"
#include "Object.h"

C_MovementAnimation::C_MovementAnimation(Object* owner) :
	Component(owner),
	m_currentState(ANIMATION_STATE::COUNT),
	m_minWalkVelocity(189.0f),
	m_minJumpVelocity(0.f)
{
}

void C_MovementAnimation::Awake()
{
	m_sprite = m_owner->GetComponent<C_AnimatedSprite>();
	m_movement = m_owner->GetComponent<C_Velocity>();
	m_mapCollision = m_owner->GetComponent<C_MapCollision>();
}

void C_MovementAnimation::Update(float deltaTime)
{
	const sf::Vector2f& velocity = m_movement->GetVelocity();
	ANIMATION_STATE animState = m_currentState;

	bool grounded = m_mapCollision->IsGrounded();

	if (grounded)
	{
		if (abs(velocity.x) > m_minWalkVelocity)
		{
			animState = ANIMATION_STATE::WALK;
		}
		else
		{
			animState = ANIMATION_STATE::IDLE;
		}
	}
	else
	{
		if (velocity.y < 0.f)
		{
			animState = ANIMATION_STATE::JUMP_UP;
		}
		else
		{
			animState == ANIMATION_STATE::WALK;
		}
	}

	// Update playing animation
	if (m_currentState != animState)
	{
		m_sprite->SetCurrentAnimation(animState);
		m_currentState = animState;
	}
}


