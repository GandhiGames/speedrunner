#include "C_BoxCollider.h"
#include "Object.h"
#include "Map.h"

C_BoxCollider::C_BoxCollider(Object* owner) : Component(owner),
m_origin(Origin::Abs_Centre), m_offset(sf::Vector2f(0.f, 0.f))
{

}


C_BoxCollider::~C_BoxCollider()
{
}

void C_BoxCollider::Awake()
{
    m_velocity = m_owner->GetComponent<C_Velocity>();
    m_events = m_owner->GetComponent<C_EntityEvents>();
}

void C_BoxCollider::Update(float deltaTime)
{
    if(!m_events) { return; }
    
    const sf::Vector2f& pos = m_owner->m_transform->GetPosition();
    const sf::Vector2f downPos = pos + sf::Vector2f(0.f,25.f);
    
    auto gameMap = m_owner->m_context.m_map;
    
    const unsigned int tileSize = gameMap->GetTileSize().x;
    
    int fromX = floor(downPos.x / tileSize);
    int fromY = floor(downPos.y / tileSize);
    
    auto tile = gameMap->GetTile(fromX, fromY);
    
    if(tile != nullptr)
    {
        m_events->m_eventSystem.Raise(m_collidingBelowEvent);
    }
}

void C_BoxCollider::SetPosition()
{
	const sf::Vector2f& pos = m_owner->m_transform->GetPosition();

	switch (m_origin)
	{
	case(Origin::Top_Left) :
	{
		m_AABB.left = pos.x + m_offset.x;
		m_AABB.top = pos.y + m_offset.y;
		break;
	}
	case(Origin::Abs_Centre) :
	{
		m_AABB.left = pos.x - (m_AABB.width / 2) + m_offset.x;
		m_AABB.top = pos.y - (m_AABB.height / 2) + m_offset.y;
		break;
	}
	case(Origin::Mid_Bottom) :
	{
		m_AABB.left = pos.x - (m_AABB.width / 2) + m_offset.x;
		m_AABB.top = pos.y - m_AABB.height + m_offset.y;
		break;
	}
	}
}

const sf::FloatRect& C_BoxCollider::GetCollidable()
{
	SetPosition();

	return m_AABB;
}

void C_BoxCollider::SetOrigin(const Origin& origin) { m_origin = origin; }

void C_BoxCollider::SetCollidable(const sf::FloatRect& rect)
{
	m_AABB = rect;
	SetPosition();
}

//TODO: intersection with circle
Manifold C_BoxCollider::Intersects(std::shared_ptr<C_Collider2D> other)
{
	std::shared_ptr<C_BoxCollider> c = std::dynamic_pointer_cast<C_BoxCollider>(other);

	Manifold m;
    m.colliding = false;
	
	if (c)
	{
		const sf::FloatRect& rect1 = GetCollidable();
		const sf::FloatRect& rect2 = c->GetCollidable();

		if (rect1.intersects(rect2))
		{
			m.colliding = true;
            m.other = rect2;
        }
	}

	return m;
}

//TODO: resolve overlap with circle
void C_BoxCollider::ResolveOverlap(const Manifold& m)
{
    auto transform = m_owner->m_transform;
    
    if(transform->isStatic()) { return; }
    
    const sf::FloatRect& rect1 = GetCollidable();
    const sf::FloatRect& rect2 = m.other;
    
    if(!rect1.intersects(rect2)) { return; }
    
    float resolve = 0;
    float xDiff = (rect1.left + (rect1.width * 0.5f)) - (rect2.left + (rect2.width * 0.5f));
    float yDiff = (rect1.top + (rect1.height * 0.5f)) - (rect2.top + (rect2.height * 0.5f));
    
    if (abs(xDiff) > abs(yDiff))
    {
        if (xDiff > 0)
        {
            resolve = (rect2.left + rect2.width) - rect1.left;
        }
        else
        {
            resolve = -((rect1.left + rect1.width) - rect2.left);
        }
        
        m_velocity->SetVelocity(0.f, m_velocity->GetVelocity().y);
        transform->AddPosition(resolve, 0);
    }
    else
    {
        if (yDiff > 0)
        {
            resolve = (rect2.top + rect2.height) - rect1.top;
        }
        else // Colliding below
        {
            resolve = -((rect1.top + rect1.height) - rect2.top);
            
            if(m_events)
            {
                m_events->m_eventSystem.Raise(m_collidingBelowEvent);
            }
        }
        
        m_velocity->SetVelocity(m_velocity->GetVelocity().x, 0.f);
        transform->AddPosition(0, resolve);
    }
    
}

