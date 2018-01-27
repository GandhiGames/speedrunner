#include "C_Health.h"
#include "Object.h"


C_Health::C_Health(Object* owner) : Component(owner), m_currentHealth(1)
{
}

void C_Health::DoDamage(const sf::Vector2f& dir, int amount)
{
	m_currentHealth -= amount;

	if (m_currentHealth < 0)
	{
		m_owner->Destroy();
	}
}

void C_Health::SetMaxHealth(int health)
{
    m_currentHealth = health;
}
