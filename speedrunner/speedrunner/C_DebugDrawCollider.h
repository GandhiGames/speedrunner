#pragma once

#include "Component.h"
#include "C_Updateable.h"
#include "C_BoxCollider.h"
#include "Debug.h" //TODO: add compiler switch/some method of disabling debug in final builds.

#include <SFML\Graphics.hpp>

class C_DebugDrawCollider : public Component, public C_Updateable
{
public:
	C_DebugDrawCollider(Object* owner);

	void Awake() override;
	void LateUpdate(float deltaTime) override;

private:
	std::shared_ptr<C_BoxCollider> m_collider;
	sf::Color m_drawColour;
};

