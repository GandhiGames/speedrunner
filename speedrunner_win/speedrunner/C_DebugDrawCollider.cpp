#include "C_DebugDrawCollider.h"
#include "Object.h"

C_DebugDrawCollider::C_DebugDrawCollider(Object* owner) : 
	Component(owner),
	m_drawColour(250, 250, 250, 120){}

void C_DebugDrawCollider::Awake()
{
	m_collider = m_owner->GetComponent<C_BoxCollider>();
}

void C_DebugDrawCollider::LateUpdate(float deltaTime)
{
	const sf::FloatRect& rect = m_collider->GetCollidable();
	Debug::DrawRect(rect, m_drawColour);
}