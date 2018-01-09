#include "C_Velocity.h"
#include "Object.h"
#include "SharedContext.h"
#include "Map.h"

C_Velocity::C_Velocity(Object* owner) : Component(owner),
m_velocity(0.f, 0.f), m_force(0.f, 0.f), m_maxVelocity(10.f, 10.f)
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

	if (abs(m_velocity.x) > 0.f || abs(m_velocity.y) > 0.f)
	{
		std::shared_ptr<C_Transform> transform = m_owner->m_transform;
		
		transform->AddPosition(m_velocity);

		Map* map = m_owner->m_context.m_map;
		const sf::Vector2u& mapSize = map->GetMapSize();
		const unsigned int tileSize = map->GetTileSize();

		const sf::Vector2f& pos = m_velocity + transform->GetPosition();

		if (pos.x < 0) 
		{
			transform->SetX(0);
		}
		else if (pos.x > (mapSize.x + 1) * tileSize)
		{
			transform->SetX ((mapSize.x + 1) * tileSize);
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

void C_Velocity::AddForce(const sf::Vector2f& force)
{
	m_force = force;
}

void C_Velocity::Set(sf::Vector2f& velocity)
{
	m_velocity = velocity;

	if (abs(m_velocity.x) > m_maxVelocity.x)
	{
		m_velocity.x = (m_velocity.x < 0.f) ? -m_maxVelocity.x : m_maxVelocity.x;
	}

	if (abs(m_velocity.y) > m_maxVelocity.y)
	{
		m_velocity.y = (m_velocity.y < 0.f) ? -m_maxVelocity.y : m_maxVelocity.y;
	}
}

const sf::Vector2f& C_Velocity::Get() const
{
	return m_velocity;
}

void C_Velocity::SetMaxVelocity(const sf::Vector2f& max)
{
	m_maxVelocity = max;
}
