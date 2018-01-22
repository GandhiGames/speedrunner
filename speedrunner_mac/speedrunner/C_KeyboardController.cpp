#include "C_KeyboardController.h"
#include "Object.h"
#include "SharedContext.h"
#include "Map.h"

C_KeyboardController::C_KeyboardController(Object* owner) : Component(owner),
m_speed(200),
m_jumpForce(400.f),
m_fallMultiplier(2.5f),
m_lowJumpMultiplier(2.f),
m_jumping(false)
{
}

void C_KeyboardController::Awake()
{
	m_movement = m_owner->GetComponent<C_Velocity>();
	m_mapCollision = m_owner->GetComponent<C_MapCollision>();
}

void C_KeyboardController::Start()
{
	float gravity = m_owner->m_context.m_map->GetGravity();
	m_movement->SetAcceleration(0.f, gravity);
}

//TODO: keyboard controller should not have calculate friction/gravity etc. 
//Move to seperate physics class.
void C_KeyboardController::Update(float timeDelta)
{
	float gravity = m_owner->m_context.m_map->GetGravity();

	CalculateMovementForce(timeDelta, gravity);
	CalculateJumpForce(timeDelta, gravity);
}

void C_KeyboardController::CalculateMovementForce(float timeDelta, float gravity)
{
	sf::Vector2f velocity = m_movement->GetVelocity();
	velocity.x = 0.f;
	if (Input::IsKeyPressed(Input::KEY::KEY_LEFT))
	{
		velocity.x = -m_speed;
	}
	else if (Input::IsKeyPressed(Input::KEY::KEY_RIGHT))
	{
		velocity.x = m_speed;
	}

	m_movement->SetVelocity(velocity);

	
	

	//TODO: implement tile and air friction
	/*
	if (m_jumping)
	{
		//TODO: load air friction in from map data file.
		const sf::Vector2f airXFriction(600.f * timeDelta, 0.f);
		m_movement->ApplyFriction(airXFriction.x, 0.f);
	}
	else
	{
		sf::Vector2f groundFriction;
		if (standingOnTile)
		{
			groundFriction = standingOnTile->m_friction * timeDelta;
		}
		else
		{
			groundFriction = m_owner->m_context.m_map->GetDefaultTile().m_friction * timeDelta;
		}

		m_movement->ApplyFriction(groundFriction.x, groundFriction.y);
	}
	*/
	//const sf::Vector2f friction = (standingOnTile) ? standingOnTile->m_friction * timeDelta : airXFriction * timeDelta;

}


//TODO: give player small amount time after walking off ledge to still be able to jump.
void C_KeyboardController::CalculateJumpForce(float timeDelta, float gravity)
{
	bool isGrounded = m_mapCollision->IsGrounded();

	if (isGrounded)
	{
		m_jumping = false;
	}

	sf::Vector2f velocity = m_movement->GetVelocity();

	if (Input::IsKeyPressed(Input::KEY::KEY_UP) && !m_jumping)
	{
		m_jumping = true;
		m_movement->SetVelocity(velocity.x, -m_jumpForce);
	}

	if (velocity.y > 0.f && !isGrounded)
	{
		velocity.y += gravity * (m_fallMultiplier - 1) * timeDelta;
		m_movement->SetVelocity(velocity.x, velocity.y);
	}
	else if (velocity.y < 0.f && !isGrounded && !Input::IsKeyPressed(Input::KEY::KEY_UP))
	{
		velocity.y += gravity * (m_lowJumpMultiplier - 1) * timeDelta;
		m_movement->SetVelocity(velocity.x, velocity.y);
	}
}

void C_KeyboardController::SetMovementSpeed(int speed)
{
	m_speed = speed;
}
