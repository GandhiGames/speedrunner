#pragma once

#include "Component.h"
#include "C_Collider2D.h"
#include "C_Velocity.h"
#include "C_EntityEvents.hpp"
#include "EventCollidingBelow.hpp"
#include "EventNotCollidingBelow.hpp"

enum class Origin { Top_Left, Abs_Centre, Mid_Bottom };

class C_BoxCollider : public Component, public C_Collider2D, public C_Updateable
{
public:
	C_BoxCollider(Object* owner);
	~C_BoxCollider();

    void Awake() override;
    
    void Update(float deltaTime) override;
    
	Manifold Intersects(std::shared_ptr<C_Collider2D> other) override;
	void ResolveOverlap(const Manifold& m) override;

	void SetCollidable(const sf::FloatRect& l_rect);
	const sf::FloatRect& GetCollidable();

	void SetOrigin(const Origin& l_origin);

private:
	void SetPosition();

private:
	sf::FloatRect m_AABB;
	Origin m_origin;
	sf::Vector2f m_offset;
    std::shared_ptr<C_Velocity> m_velocity;
    std::shared_ptr<C_EntityEvents> m_events;
    EventCollidingBelow m_collidingBelowEvent;
    EventNotCollidingBelow m_notCollidingBelowEvent;
};

