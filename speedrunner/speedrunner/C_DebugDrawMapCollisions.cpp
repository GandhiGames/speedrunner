#include "C_DebugDrawMapCollisions.h"
#include "Object.h"

C_DebugDrawMapCollisions::C_DebugDrawMapCollisions(Object* owner) : 
	Component(owner),
	m_drawColour(250, 250, 250, 120){}

void C_DebugDrawMapCollisions::Awake()
{
	m_mapCollisions = m_owner->GetComponent<C_MapCollision>();
}

void C_DebugDrawMapCollisions::LateUpdate(float deltaTime)
{
	const std::vector<MapCollisionElement>& collisions = m_mapCollisions->GetCollisions();

	for (const auto& c : collisions)
	{
		Debug::DrawRect(c.m_tileBounds, m_drawColour);
	}
}