#include "C_DirectionalAnimation.h"

#include "C_DirectionalAnimation.h"
#include "Object.h"

C_DirectionalAnimation::C_DirectionalAnimation(Object* owner) : Component(owner),
m_currentState(ANIMATION_STATE::COUNT)
{
}

void C_DirectionalAnimation::Awake()
{
	m_sprite = m_owner->GetComponent<C_AnimatedSprite>();
	m_movement = m_owner->GetComponent<C_Velocity>();
}

void C_DirectionalAnimation::Update(float deltaTime)
{
	ANIMATION_STATE animState = m_currentState;

	const sf::Vector2f& velocity = m_movement->GetVelocity();

	if (velocity.x != 0)
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

