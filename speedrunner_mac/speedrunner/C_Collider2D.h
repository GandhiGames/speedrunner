#pragma once

#include<memory>
#include <SFML/Graphics.hpp>

#include "Bitmask.h"

// enum ints used when defining collision layers
enum class COLLISION_LAYER
{
	DEFAULT = 0,
	PLAYER = 1,
	FOLLOWERS = 2,
    PROJECTILE = 3,
    BOSS = 4
};

struct Manifold
{
	bool colliding = false;
    sf::FloatRect m_intersection;
};

class C_Collider2D 
{
public:
	C_Collider2D();
	~C_Collider2D();

	virtual Manifold Intersects(std::shared_ptr<C_Collider2D> other) = 0;
	virtual void ResolveOverlap(const Manifold& m) = 0;

	bool IsTrigger() const;
	void SetTrigger(bool isTrigger);

	COLLISION_LAYER GetLayer() const;
	void SetLayer(COLLISION_LAYER layer);

private:
	bool m_isTrigger;
	COLLISION_LAYER m_layer;
};

