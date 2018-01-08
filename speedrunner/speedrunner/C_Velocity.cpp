#include "C_Velocity.h"
#include "Object.h"
#include "SharedContext.h"

C_Velocity::C_Velocity(Object* owner) : Component(owner),
m_velocity({ 0.f, 0.f }), m_force({0.f, 0.f})
{
}



void C_Velocity::Update(float timeDelta)
{
	//TODO: move force calculations to fixed update method (create fixed update method!)
	if (abs(m_force.x) > 0.f || abs(m_force.y) > 0.f)
	{
		m_velocity += m_force * timeDelta;

		m_force *= 0.9f;
	}

	m_owner->m_transform->AddPosition(m_velocity);
}

void C_Velocity::AddForce(const sf::Vector2f& force)
{
	m_force = force;
}

void C_Velocity::Set(sf::Vector2f& velocity)
{
	if (abs(velocity.x) > 0.f || abs(velocity.y) > 0.f)
	{
		//TODO: level collisions.

		m_velocity = velocity;
	}
	else
	{
		m_velocity = velocity;
	}

}

const sf::Vector2f& C_Velocity::Get() const
{
	return m_velocity;
}
