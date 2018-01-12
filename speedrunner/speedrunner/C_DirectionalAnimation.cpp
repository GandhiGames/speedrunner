#include "C_DirectionalAnimation.h"

#include "C_DirectionalAnimation.h"
#include "Object.h"

C_DirectionalAnimation::C_DirectionalAnimation(Object* owner) : 
	Component(owner),
	m_currentState(ANIMATION_STATE::COUNT),
	m_minWalkVelocity(1.8f)
{
}

void C_DirectionalAnimation::Awake()
{
	m_sprite = m_owner->GetComponent<C_AnimatedSprite>();
	m_movement = m_owner->GetComponent<C_Velocity>();
}

void C_DirectionalAnimation::Update(float deltaTime)
{
	const sf::Vector2f& velocity = m_movement->GetVelocity();
	ANIMATION_STATE animState = m_currentState;

	if (abs(velocity.x) > m_minWalkVelocity)
	{
		animState = ANIMATION_STATE::WALK;
	}
	else
	{
		animState = ANIMATION_STATE::IDLE;
	}

	// Update playing animation
	if (m_currentState != animState)
	{
		m_sprite->SetCurrentAnimation(animState);
		m_currentState = animState;
	}
}


