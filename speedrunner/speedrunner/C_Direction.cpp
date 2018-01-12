#include "C_Direction.h"
#include "Object.h"
#include "Debug.h"

C_Direction::C_Direction(Object* owner) :
	Component(owner),
	m_moveDir(MOVEMENT_DIRECTION::RIGHT),
	m_cached(false)
{
	//TODO: creates issue when components added out of order. Create Awake/Start methods and move these to them.
	m_movement = owner->GetComponent<C_Velocity>();

	if (!m_movement)
	{
		Debug::LogError("Requires velocity component to calculate direction");
	}
}

//TODO: this can introduce a bug if a movement direction is requested in lateupdate.
void C_Direction::LateUpdate(float timeDelta)
{
	m_cached = false;
}

MOVEMENT_DIRECTION C_Direction::Get()
{
	if (!m_cached)
	{
		const sf::Vector2f& velocity = m_movement->GetVelocity();

		if (velocity.x != 0)
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

		m_cached = true;
	}

	return m_moveDir;
}