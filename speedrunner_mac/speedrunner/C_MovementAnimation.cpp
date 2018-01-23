#include "C_MovementAnimation.h"

#include "C_MovementAnimation.h"
#include "Object.h"

C_MovementAnimation::C_MovementAnimation(Object* owner) :
	Component(owner),
	m_minWalkVelocity(0.0f),
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
	ANIMATION_STATE animState = m_sprite->GetAnimationState();
	
	//TODO: look into a better way of checking whether an animation can be set. Maybe event system? entity flags?
	if (animState == ANIMATION_STATE::ATTACK_IN_AIR || animState == ANIMATION_STATE::ATTACK_ON_GROUND)
	{
		return;
	}

	const sf::Vector2f& velocity = m_movement->GetVelocity();

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
        if(abs(velocity.x) > 0.f)
        {
            animState = ANIMATION_STATE::JUMP_ANGLED_UP;
        }
        else
        {
            animState = ANIMATION_STATE::JUMP_STRAIGHT_UP;
        }
        
	}

	m_sprite->SetAnimationState(animState);
}


