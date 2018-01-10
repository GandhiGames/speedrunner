#include "C_KeyboardController.h"
#include "Object.h"
#include "SharedContext.h"
#include "Map.h"

C_KeyboardController::C_KeyboardController(Object* owner) : Component(owner), 
m_speed(200)
{
}


void C_KeyboardController::Awake()
{
	m_movement = m_owner->GetComponent<C_Velocity>();
}

void C_KeyboardController::Update(float timeDelta)
{
	sf::Vector2f movementSpeed(0.f, 0.f);

	if (Input::IsKeyPressed(Input::KEY::KEY_LEFT))
	{
		movementSpeed.x = -m_speed;
	}
	else if (Input::IsKeyPressed(Input::KEY::KEY_RIGHT))
	{
		movementSpeed.x = m_speed;
	}

	//TODO: keyboard controller should not have calculate friction/gravity etc. 
	//Move to seperate physics class.
	Map* map = m_owner->m_context.m_map;

	float gravity = map->GetGravity();
	m_movement->AddForce(0, gravity);

	m_movement->SetVelovity(movementSpeed * timeDelta);

	//TODO: get current tile friction.
	const sf::Vector2f& tileFriction = map->GetDefaultTile().m_friction;
	float frictionX = (movementSpeed.x * tileFriction.x) * timeDelta; 
	float frictionY = (movementSpeed.y * tileFriction.y) * timeDelta; 
	m_movement->ApplyFriction(frictionX, frictionY);
}

void C_KeyboardController::SetMovementSpeed(int speed)
{
	m_speed = speed;
}