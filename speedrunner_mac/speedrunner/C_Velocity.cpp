#include "C_Velocity.h"
#include "Object.h"
#include "SharedContext.h"
#include "Map.h"

//TODO: rename to physicsbody/simplephysicsbody
C_Velocity::C_Velocity(Object* owner) : Component(owner),
m_velocity(0.f, 0.f),
m_force(0.f, 0.f),
m_acceleration(0.f, 0.f),
m_maxVelocity(300.f, 980.f),
m_inverseMass(0.9f),
m_damping(0.89f),
m_blockX(false),
m_blockY(false)
{
}

void C_Velocity::Update(float timeDelta)
{
	if (m_inverseMass < 0.0f) { return; }

    m_owner->m_transform->AddPosition(m_velocity * timeDelta);
	//ClampToMap();

	sf::Vector2f resultingAcc = m_acceleration;
	resultingAcc += m_force * m_inverseMass;

	m_velocity += resultingAcc * timeDelta;

	ClampVelocity();

	m_velocity *= powf(m_damping, timeDelta);

	m_force.x = 0.f;
	m_force.y = 0.f;
}

//TODO: do we want map clamp in velocity class? could move to C_Mapcollisions
void C_Velocity::ClampToMap()
{

	Map* map = m_owner->m_context.m_map;
	//TODO: these can be cached until map changes.
	const sf::Vector2u& mapSize = map->GetMapSize();
	const unsigned int tileSize = map->GetTileSize().x;

	std::shared_ptr<C_Transform> transform = m_owner->m_transform;

	const sf::Vector2f& pos = transform->GetPosition();

	if (pos.x < 0)
	{
		m_velocity.x = 0;
		transform->SetX(0);
	}
	else if (pos.x >(mapSize.x + 1) * tileSize)
	{
		m_velocity.x = 0;
		transform->SetX((mapSize.x + 1) * tileSize);
	}

	if (pos.y < 0)
	{
		m_velocity.y = 0;
		transform->SetY(0);
	}
	else if (pos.y >(mapSize.y + 1) * tileSize)
	{
		m_velocity.y = 0;
		transform->SetY((mapSize.y + 1) * tileSize);
		//TODO: should die here. Don't want velocity knowing this so move to different class.
	}
}

void C_Velocity::SetAcceleration(float x, float y)
{
	m_acceleration.x = x;
	m_acceleration.y = y;
}

void C_Velocity::SetAcceleration(const sf::Vector2f& acceleration)
{
	m_acceleration = acceleration;
}

const sf::Vector2f& C_Velocity::GetAcceleration() const
{
	return m_acceleration;
}

void C_Velocity::AddForce(float x, float y)
{
	m_force.x += x;
	m_force.y += y;
}

void C_Velocity::AddForce(const sf::Vector2f& force)
{
	m_force += force;
}

void C_Velocity::AddVelocity(float x, float y)
{
	SetVelocityX(m_velocity.x = x);
	SetVelocityY(m_velocity.y = y);
    
    ClampVelocity();
}

void C_Velocity::AddVelocity(sf::Vector2f& velocity)
{
    const sf::Vector2f newVel = m_velocity + velocity;
    SetVelocityX(newVel.x);
    SetVelocityY(newVel.y);
    
	ClampVelocity();
}

void C_Velocity::SetVelocity(const sf::Vector2f& velocity)
{
    SetVelocityX(velocity.x);
    SetVelocityY(velocity.y);

	ClampVelocity();
}

void C_Velocity::SetVelocity(float x, float y)
{
    SetVelocityX(x);
    SetVelocityY(y);
    
	ClampVelocity();
}

void C_Velocity::SetVelocityX(float x)
{
    if(!m_blockX)
    {
        m_velocity.x = x;
    }
}

void C_Velocity::SetVelocityY(float y)
{
    if(!m_blockY)
    {
        m_velocity.y = y;
    }
}

void C_Velocity::BlockX()
{
    m_blockX = true;
}

void C_Velocity::BlockY()
{
    m_blockY = true;
}

void C_Velocity::AllowX()
{
    m_blockX = false;
}

void C_Velocity::AllowY()
{
    m_blockY = false;
}

void C_Velocity::ClampVelocity()
{
	if (abs(m_velocity.x) > m_maxVelocity.x)
	{
		if (m_velocity.x > 0.f)
		{
			m_velocity.x = m_maxVelocity.x;
		}
		else
		{
			m_velocity.x = -m_maxVelocity.x;
		}
	}

	if (abs(m_velocity.y) > m_maxVelocity.y)
	{
		if (m_velocity.y > 0.f)
		{
			m_velocity.y = m_maxVelocity.y;
		}
		else
		{
			m_velocity.y = -m_maxVelocity.y;
		}
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
			if (m_velocity.x < 0)
			{
				m_velocity.x += x;
			}
			else
			{
				m_velocity.x -= x;
			}
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
			if (m_velocity.y < 0)
			{
				m_velocity.y += y;
			}
			else
			{
				m_velocity.y -= y;
			}
		}
	}
}

const sf::Vector2f& C_Velocity::GetVelocity() const
{
	return m_velocity;
}

void C_Velocity::SetMaxVelocity(float x, float y)
{
	m_maxVelocity.x = x;
	m_maxVelocity.y = y;
}

void C_Velocity::SetMaxVelocity(const sf::Vector2f& max)
{
	m_maxVelocity = max;
}
