#include "C_MapCollision.h"
#include "Object.h"
#include "Map.h"

C_MapCollision::C_MapCollision(Object* owner) : 
	Component(owner),
	m_collidingOnX(false),
	m_collidingOnY(false)
{
}


C_MapCollision::~C_MapCollision()
{
}

void C_MapCollision::Awake()
{
	m_collider = m_owner->GetComponent<C_BoxCollider>();

	if (!m_collider)
	{
		Debug::LogError("No collider found for map collisions");
	}

	m_velocity = m_owner->GetComponent<C_Velocity>();

	if (!m_velocity)
	{
		Debug::LogError("No velocity found for map collisions");
	}
}

void C_MapCollision::Update(float deltaTime)
{
	Map* gameMap = m_owner->m_context.m_map;
	//TODO: cache this.
	const sf::FloatRect& AABB = m_collider->GetCollidable();
	CheckCollisions(gameMap, AABB);
	ResolveCollisions(gameMap);
}

void C_MapCollision::CheckCollisions(Map* gameMap, const sf::FloatRect& AABB)
{
	unsigned int tileSize = gameMap->GetTileSize();

	int fromX = floor(AABB.left / tileSize);
	int toX = floor((AABB.left + AABB.width) / tileSize);

	int fromY = floor(AABB.top / tileSize);
	int toY = floor((AABB.top + AABB.height) / tileSize);

	for (int x = fromX; x <= toX; ++x)
	{
		for (int y = fromY; y <= toY; ++y)
		{
			std::shared_ptr<Tile> tile = gameMap->GetTile(x, y);
			if (!tile) { continue; }

			sf::FloatRect tileBounds(x * tileSize, y * tileSize, tileSize, tileSize);
			sf::FloatRect intersection;
			AABB.intersects(tileBounds, intersection); // Returns the overlapped rectangle in the intersection parameter.

			float area = intersection.width * intersection.height;
			MapCollisionElement e(area, tile->m_properties, tileBounds);
			m_collisions.emplace_back(e);

			//TODO: do we want to implement level warp?
			/*
			if (tile->m_warp && m_type == EntityType::Player)
			{
			gameMap->LoadNext();
			}
			*/
		}
	}
}

void C_MapCollision::ResolveCollisions(Map* gameMap)
{
	if (!m_collisions.empty())
	{
		std::sort(m_collisions.begin(), m_collisions.end(), SortCollisions);

		unsigned int tileSize = gameMap->GetTileSize();
		for (auto &itr : m_collisions)
		{
			const sf::FloatRect& AABB = m_collider->GetCollidable();
			if (!AABB.intersects(itr.m_tileBounds)) { continue; }

			float xDiff = (AABB.left + (AABB.width / 2)) - (itr.m_tileBounds.left + (itr.m_tileBounds.width / 2));
			float yDiff = (AABB.top + (AABB.height / 2)) - (itr.m_tileBounds.top + (itr.m_tileBounds.height / 2));

			float resolve = 0;

			if (abs(xDiff) > abs(yDiff))
			{
				if (xDiff > 0)
				{
					resolve = (itr.m_tileBounds.left + tileSize) - AABB.left;
				}
				else
				{
					resolve = -((AABB.left + AABB.width) - itr.m_tileBounds.left);
				}

				m_velocity->Move(resolve, 0);

				m_velocity->SetVelocity(0.f, m_velocity->GetVelocity().y);

				m_collidingOnX = true;
			}
			else 
			{ 
				if (yDiff > 0) 
				{ 
					resolve = (itr.m_tileBounds.top + tileSize) - AABB.top; 
				} 
				else 
				{ 
					resolve = -((AABB.top + AABB.height) - itr.m_tileBounds.top); 
				} 
				
				m_velocity->Move(0, resolve);

				m_velocity->SetVelocity(m_velocity->GetVelocity().x, 0.f);
				
				if (m_collidingOnY) { continue; } 
				
				m_standingOnTile = itr.m_tile;
				
				m_collidingOnY = true; 
			}
		} 
		m_collisions.clear();
	} 

	if (!m_collidingOnY) 
	{
		m_standingOnTile = nullptr;
	}
}

bool C_MapCollision::SortCollisions(const MapCollisionElement& a, const MapCollisionElement& b)
{
	return a.m_area > b.m_area;
}
