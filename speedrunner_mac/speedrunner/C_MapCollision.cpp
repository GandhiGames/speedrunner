#include "C_MapCollision.h"
#include "Object.h"
#include "Map.h"

C_MapCollision::C_MapCollision(Object* owner) : 
	Component(owner),
	m_collidingOnX(false),
	m_collidingOnY(false)
	//m_grounded(false)
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
	/* 
		Collisions are cleared at the beginning of the next update (rather than the end of the previous update)
		so debug can use the collision data in late update.
	*/
	m_collisions.clear(); 

	Map* gameMap = m_owner->m_context.m_map;
	
	CheckCollisions(gameMap);
	ResolveCollisions(gameMap);
}

void C_MapCollision::CheckCollisions(Map* gameMap)
{
	//m_grounded = false;

	const unsigned int tileSize = gameMap->GetTileSize().x;
	const float halfTileSize = tileSize * 0.5f;
	const sf::FloatRect& AABB = m_collider->GetCollidable();

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
			m_collisions.emplace_back(MapCollisionElement(area, tile->m_properties, tileBounds));
		}
	}
}

void C_MapCollision::ResolveCollisions(Map* gameMap)
{
	m_collidingOnX = false;
	m_collidingOnY = false;
	m_standingOnTile = nullptr;

	if (!m_collisions.empty())
	{
		std::sort(m_collisions.begin(), m_collisions.end(), SortCollisions);

		for (auto &itr : m_collisions)
		{
			const sf::FloatRect& AABB = m_collider->GetCollidable();
			if (!AABB.intersects(itr.m_tileBounds)) { continue; }

			float resolve = 0;
			float xDiff = (AABB.left + (AABB.width * 0.5f)) - (itr.m_tileBounds.left + (itr.m_tileBounds.width * 0.5f));
			float yDiff = (AABB.top + (AABB.height * 0.5f)) - (itr.m_tileBounds.top + (itr.m_tileBounds.height * 0.5f));

            //TODO: duplicate code in C_boxcollider
			if (abs(xDiff) > abs(yDiff))
			{
				if (xDiff > 0)
				{
					resolve = (itr.m_tileBounds.left + itr.m_tileBounds.width) - AABB.left;
				}
				else
				{
					resolve = -((AABB.left + AABB.width) - itr.m_tileBounds.left);
				}

                m_owner->m_transform->AddPosition(resolve, 0);
                
                sf::Vector2f vel = m_velocity->GetVelocity();
                if(resolve < 0.f && vel.x > 0.f)
                {
                    m_velocity->SetVelocity(0.f, m_velocity->GetVelocity().y);
                }
                else if(resolve > 0.f && vel.x < 0.f)
                {
                    m_velocity->SetVelocity(0.f, m_velocity->GetVelocity().y);
                }

				m_collidingOnX = true;
			}
			else 
			{ 
				if (yDiff > 0) 
				{ 
					resolve = (itr.m_tileBounds.top + itr.m_tileBounds.height) - AABB.top; 
				} 
				else 
				{ 
					resolve = -((AABB.top + AABB.height) - itr.m_tileBounds.top); 
				} 
				
                m_owner->m_transform->AddPosition(0, resolve);
                
                sf::Vector2f vel = m_velocity->GetVelocity();
                if(resolve < 0.f && vel.y > 0.f)
                {
                    m_velocity->SetVelocity(m_velocity->GetVelocity().x, 0.f);
                }
                else if(resolve > 0.f && vel.y < 0.f)
                {
                    m_velocity->SetVelocity(m_velocity->GetVelocity().x, 0.f);
                }
			
				if (m_collidingOnY) { continue; } 
				
				if (yDiff < 0.f) // If tile below player
				{
					m_standingOnTile = itr.m_tile;
				}
				
				m_collidingOnY = true; 
			}
		} 
	} 
}

const std::vector<MapCollisionElement>& C_MapCollision::GetCollisions() const
{
	return m_collisions;
}

bool C_MapCollision::SortCollisions(const MapCollisionElement& a, const MapCollisionElement& b)
{
	return a.m_area > b.m_area;
}

//TODO: this does not always return the tile below. Even if the player does not move, different frames
// results in different returns.
std::shared_ptr<TileInfo> C_MapCollision::GetTileBelow()
{
	return m_standingOnTile;
}


