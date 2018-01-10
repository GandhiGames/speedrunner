#include "C_Velocity.h"
#include "Object.h"
#include "SharedContext.h"
#include "Map.h"

C_Velocity::C_Velocity(Object* owner) : Component(owner),
m_velocity(0.f, 0.f),
//m_force(0.f, 0.f), 
m_maxVelocity(10.f, 10.f)
{
}



void C_Velocity::Update(float timeDelta)
{
	/*
	//TODO: move force calculations to fixed update method (create fixed update method!)
	if (abs(m_force.x) > 0.f || abs(m_force.y) > 0.f)
	{
		m_velocity += m_force * timeDelta;

		//m_force *= 0.9f;
		m_force.x = 0.f;
		m_force.y = 0.f;
	}
	*/


	if (abs(m_velocity.x) > 0.f || abs(m_velocity.y) > 0.f)
	{
		std::shared_ptr<C_Transform> transform = m_owner->m_transform;

		Move(m_velocity.x, m_velocity.y);

		//TODO: do we want map clamp in velocity class?
		Map* map = m_owner->m_context.m_map;

		//TODO: these can be stored.
		const sf::Vector2u& mapSize = map->GetMapSize();
		const unsigned int tileSize = map->GetTileSize();

		const sf::Vector2f& pos = transform->GetPosition();

		if (pos.x < 0)
		{
			transform->SetX(0);
		}
		else if (pos.x > (mapSize.x + 1) * tileSize)
		{
			transform->SetX((mapSize.x + 1) * tileSize);
		}

		if (pos.y < 0)
		{
			transform->SetY(0);
		}
		else if (pos.y > (mapSize.y + 1) * tileSize)
		{
			transform->SetY((mapSize.y + 1) * tileSize);
			//TODO: should die here. Don't want velocity knowing this so move to different class.
		}
	}
}

/*
void C_Velocity::AddForce(float x, float y)
{
	m_force.x += x;
	m_force.y += y;
}

void C_Velocity::AddForce(const sf::Vector2f& force)
{
	m_force += force;
}
*/

void C_Velocity::AddVelovity(sf::Vector2f& velocity)
{
	m_velocity += velocity;

	ClampVelocity();
}

void C_Velocity::SetVelocity(sf::Vector2f& velocity)
{
	m_velocity = velocity;

	ClampVelocity();
}

void C_Velocity::SetVelocity(float x, float y)
{
	m_velocity.x = x;
	m_velocity.y = y;

	ClampVelocity();
}

void C_Velocity::ClampVelocity()
{
	if (abs(m_velocity.x) > m_maxVelocity.x)
	{
		m_velocity.x = (m_velocity.x < 0.f) ? -m_maxVelocity.x : m_maxVelocity.x;
	}

	if (abs(m_velocity.y) > m_maxVelocity.y)
	{
		m_velocity.y = (m_velocity.y < 0.f) ? -m_maxVelocity.y : m_maxVelocity.y;
	}
}

void C_Velocity::ApplyFriction(float x, float y)
{
	if (x != 0 && m_velocity.x != 0)
	{
		if (abs(m_velocity.x) - abs(x) < 0)
		{
			m_velocity.x = 0;
		}
		else
		{
			m_velocity.x += x;
		}
	}

	//TODO: this is copied and pasted from above (just editing y value). Remove duplication
	if (y != 0 && m_velocity.y != 0)
	{
		if (abs(m_velocity.y) - abs(y) < 0)
		{
			m_velocity.y = 0;
		}
		else
		{
			m_velocity.y += y;
		}
	}
}

const sf::Vector2f& C_Velocity::GetVelocity() const
{
	return m_velocity;
}

void C_Velocity::SetMaxVelocity(const sf::Vector2f& max)
{
	m_maxVelocity = max;
}

void C_Velocity::Move(float x, float y)
{
	m_owner->m_transform->AddPosition(x, y);
}