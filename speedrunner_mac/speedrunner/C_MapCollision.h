#pragma once

#include <memory>
#include <algorithm>

#include "C_Updateable.h"
#include "C_BoxCollider.h"
#include "C_Velocity.h"
#include "Tile.h"

struct MapCollisionElement
{
	MapCollisionElement(float area, std::shared_ptr<TileInfo> info, const sf::FloatRect& bounds) :
		m_area(area),
		m_tile(info),
		m_tileBounds(bounds) {}
	
	
	float m_area; 
	std::shared_ptr<TileInfo> m_tile;
	sf::FloatRect m_tileBounds;
};

class C_MapCollision : public Component, public C_Updateable
{
public:
	C_MapCollision(Object* owner);
	~C_MapCollision();

	void Awake() override;

	void Update(float deltaTime) override;

	std::shared_ptr<TileInfo> GetTileBelow();

	const std::vector<MapCollisionElement>& GetCollisions() const;

	bool IsGrounded() const;

private:
	static bool SortCollisions(const MapCollisionElement& a, const MapCollisionElement& b);
	void CheckCollisions(Map* gameMap);
	void ResolveCollisions(Map* gameMap);

private:
	//TODO: make generic and work with C_Collider2D.
	std::shared_ptr<C_BoxCollider> m_collider;
	std::shared_ptr<C_Velocity> m_velocity;
	std::vector<MapCollisionElement> m_collisions;
	std::shared_ptr<TileInfo> m_standingOnTile;
	bool m_collidingOnX;
	bool m_collidingOnY;
};

