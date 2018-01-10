#include "C_KeyboardController.h"
#include "Object.h"
#include "SharedContext.h"
#include "Map.h"

C_KeyboardController::C_KeyboardController(Object* owner) : Component(owner), 
m_speed(100), m_jumpForce(800)
{
}


void C_KeyboardController::Awake()
{
	m_movement = m_owner->GetComponent<C_Velocity>();
}

void C_KeyboardController::Update(float timeDelta)
{
	sf::Vector2f movement(0.f, 0.f);
	//sf::Vector2f force(0.f, 0.f);

	if (Input::IsKeyPressed(Input::KEY::KEY_LEFT))
	{
		movement.x = -m_speed;
	}
	else if (Input::IsKeyPressed(Input::KEY::KEY_RIGHT))
	{
		movement.x = m_speed;
	}

	if (Input::IsKeyPressed(Input::KEY::KEY_UP))
	{
		movement.y = -m_jumpForce;
	}

	//TODO: keyboard controller should not have calculate friction/gravity etc. 
	//Move to seperate physics class.
	Map* map = m_owner->m_context.m_map;

	float gravity = map->GetGravity();
	
	m_movement->ApplyFriction(-m_movement->GetVelocity().x * 0.1f, 0.f);

	sf::Vector2f velocity = movement;
	velocity.y += gravity;
	velocity *= timeDelta;
	m_movement->AddVelovity(velocity);

	/*
	//TODO: get current tile friction.
	const sf::Vector2f& tileFriction = map->GetDefaultTile().m_friction;
	float frictionX = (movement.x * tileFriction.x) * timeDelta; 
	float frictionY = (movement.y * tileFriction.y) * timeDelta; 
	m_movement->ApplyFriction(frictionX, frictionY);
	*/
}

void C_KeyboardController::SetMovementSpeed(int speed)
{
	m_speed = speed;
}