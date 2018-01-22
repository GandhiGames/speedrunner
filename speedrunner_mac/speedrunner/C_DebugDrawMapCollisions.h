#pragma once

#include "C_MapCollision.h"

class C_DebugDrawMapCollisions : public Component, public C_Updateable
{
public:
	C_DebugDrawMapCollisions(Object* owner);
	
	void Awake() override;
	void LateUpdate(float deltaTime) override;

private:
	std::shared_ptr<C_MapCollision> m_mapCollisions;
	sf::Color m_drawColour;
};

