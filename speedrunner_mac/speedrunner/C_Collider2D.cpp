#include "C_Collider2D.h"

C_Collider2D::C_Collider2D() : m_isTrigger(false), m_layer(COLLISION_LAYER::DEFAULT)
{
}


C_Collider2D::~C_Collider2D()
{
}

bool C_Collider2D::IsTrigger() const
{
	return m_isTrigger;
}

void C_Collider2D::SetTrigger(bool isTrigger)
{
	m_isTrigger = isTrigger;
}

COLLISION_LAYER C_Collider2D::GetLayer() const
{
	return m_layer;
}

void C_Collider2D::SetLayer(COLLISION_LAYER layer)
{
	m_layer = layer;
}
