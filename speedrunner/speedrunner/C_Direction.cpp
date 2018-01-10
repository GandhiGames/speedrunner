#include "C_Direction.h"
#include "Object.h"
#include "Debug.h"

C_Direction::C_Direction(Object* owner) : Component(owner), m_moveDir(MOVEMENT_DIRECTION::RIGHT)
{
	//TODO: creates issue when components added out of order. Create Awake/Start methods and move these to them.
	m_movement = owner->GetComponent<C_Velocity>();

	if (!m_movement)
	{
		Debug::LogError("Requires velocity component to calculate direction");
	}
}

void C_Direction::Update(float timeDelta)
{
	const sf::Vector2f& velocity = m_movement->GetVelocity();

	if ((velocity.x != 0) || (velocity.y != 0))
	{
		if (abs(velocity.x) > abs(velocity.y))
		{
			if (velocity.x <= 0)
			{
				m_moveDir = MOVEMENT_DIRECTION::LEFT;
			}
			else
			{
				m_moveDir = MOVEMENT_DIRECTION::RIGHT;
			}
		}
	}
}

MOVEMENT_DIRECTION C_Direction::Get()
{
	return m_moveDir;
}