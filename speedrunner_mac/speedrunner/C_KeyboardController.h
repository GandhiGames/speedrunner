#pragma once

#include "Component.h"
#include "C_Updateable.h"
#include "C_Velocity.h"
#include "C_MapCollision.h"
#include "Input.h"
#include "C_EntityEvents.hpp"
#include "EventCollidingBelow.hpp"

//TODO: rename to movementcontroller
class C_KeyboardController : public Component, public C_Updateable
{
	friend class Object;

public:
	C_KeyboardController(Object* owner);
	
	void Awake() override;
	void Start() override;

	void Update(float timeDelta) override;

	void SetMovementSpeed(int speed);
    
    void OnCollidingBelow(const Event& e);
    void OnNotCollidingBelow(const Event& e);
    
private:
	void CalculateMovementForce(float timeDelta, float gravity);
	void CalculateJumpForce(float timeDelta, float gravity);

private:
	std::shared_ptr<C_Velocity> m_movement;
	std::shared_ptr<C_MapCollision> m_mapCollision;
    std::shared_ptr<C_EntityEvents> m_events;
    //TODO: should keyboard controller decide movement speed? nope.
	int m_speed;
    sf::Vector2f m_jumpForce;
	float m_fallMultiplier;
	float m_lowJumpMultiplier;
	bool m_jumping;
    int m_jumpCount; // TODO: make const
    int m_currentJumpCount;
    bool m_grounded;
};

