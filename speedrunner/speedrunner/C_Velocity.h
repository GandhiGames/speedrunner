#pragma once

#include "C_Transform.h"
#include "C_Updateable.h"

class C_Velocity : public Component, public C_Updateable
{
	friend class Object;

public:
	C_Velocity(Object* owner);

	void Update(float timeDelta) override;

	void AddForce(float x, float y);
	void AddForce(const sf::Vector2f& force);
	
	void SetVelovity(sf::Vector2f& velocity);
	const sf::Vector2f& GetVelocity() const;

	void SetMaxVelocity(const sf::Vector2f& max);

	void ApplyFriction(int x, int y);

private:
	sf::Vector2f m_velocity;
	sf::Vector2f m_force;
	sf::Vector2f m_maxVelocity;
};


