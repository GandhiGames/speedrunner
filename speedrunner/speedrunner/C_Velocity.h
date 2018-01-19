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
	
	void AddVelocity(float x, float y);
	void AddVelocity(sf::Vector2f& velocity);

	void SetVelocity(const sf::Vector2f& velocity);
	void SetVelocity(float x, float y);
	const sf::Vector2f& GetVelocity() const;

	void SetMaxVelocity(float x, float y);
	void SetMaxVelocity(const sf::Vector2f& max);

	void ApplyFriction(float x, float y);

	void Move(const sf::Vector2f& move);
	void Move(float x, float y);

	void SetAcceleration(float x, float y);
	void SetAcceleration(const sf::Vector2f& acceleration);
	const sf::Vector2f& GetAcceleration();

private:
	void ClampVelocity();
	void ClampToMap();

private:
	sf::Vector2f m_velocity;
	sf::Vector2f m_force;
	sf::Vector2f m_acceleration;

	//TODO: set max force and acceleration?
	sf::Vector2f m_maxVelocity;

	float m_inverseMass;
	float damping;
};


