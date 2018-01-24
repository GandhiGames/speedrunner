#include "CollisionResolver.h"

CollisionResolver::CollisionResolver()
{
    COLLISION_LAYERS.insert(std::make_pair(COLLISION_LAYER::DEFAULT, Bitmask(0)));
    COLLISION_LAYERS.insert(std::make_pair(COLLISION_LAYER::PLAYER, Bitmask(0)));
    COLLISION_LAYERS.insert(std::make_pair(COLLISION_LAYER::FOLLOWERS, Bitmask(0)));
    COLLISION_LAYERS.insert(std::make_pair(COLLISION_LAYER::BOSS, Bitmask(0)));
    COLLISION_LAYERS.insert(std::make_pair(COLLISION_LAYER::PROJECTILE, Bitmask(16)));
}


CollisionResolver::~CollisionResolver()
{
}

void CollisionResolver::ProcessRemovals()
{
	ProcessCollisionExitDueToRemovals();

	for (auto& layer : m_collidables)
	{
		auto itr = layer.second.begin();
		while (itr != layer.second.end())
		{
			if ((*itr)->IsQueuedForRemoval())
			{
				itr = layer.second.erase(itr);
			}
			else
			{
				++itr;
			}
		}
	}
}

void CollisionResolver::ProcessNewObjects()
{
	auto objects = Object::GetNewObjects();

	if (objects.size() == 0)
	{
		return;
	}

	for (auto& obj : objects)
	{
		auto collider = obj->GetComponent<C_Collider2D>();
		if (collider)
		{
			COLLISION_LAYER layer = collider->GetLayer();

			auto itr = m_collidables.find(layer);

			if (itr != m_collidables.end())
			{
				m_collidables[layer].push_back(obj);
			}
			else
			{
				std::vector<std::shared_ptr<Object>> objs;
				objs.push_back(obj);

				m_collidables.insert(std::make_pair(layer, objs));
			}
		}
	}
}

//TODO: create spatial position data structure. No need to check every collidable with every other collidable
//https://gamedevelopment.tutsplus.com/tutorials/quick-tip-use-quadtrees-to-detect-likely-collisions-in-2d-space--gamedev-374
void CollisionResolver::Resolve()
{
	ProcessCollisionExits();

	for (auto maps = m_collidables.begin(); maps != m_collidables.end(); ++maps)
	{
		for (auto maps2 = maps; maps2 != m_collidables.end(); ++maps2)
		{
            if(maps == maps2) { continue; }
            
			bool firstSecond = COLLISION_LAYERS[maps->first].GetBit((int)maps2->first);
			bool secondFirst = COLLISION_LAYERS[maps2->first].GetBit((int)maps->first);
			
            if (firstSecond)
			{
				ProcessCollisions(maps->second, maps2->second);
			}

			if (secondFirst)
			{
				ProcessCollisions(maps2->second, maps->second);
			}

        }
	}
}

void CollisionResolver::ProcessCollisions(std::vector<std::shared_ptr<Object>>& first, std::vector<std::shared_ptr<Object>>& second)
{
	for (auto itr = first.begin(); itr != first.end(); ++itr)
	{
		auto first = (*itr);
		auto collider1 = first->GetComponent<C_Collider2D>();

		for (auto itr2 = second.begin(); itr2 != second.end(); ++itr2)
		{
			auto second = (*itr2);

			if (first->m_instanceID->Get() == second->m_instanceID->Get())
			{
				continue;
			}

			auto collider2 = (*itr2)->GetComponent<C_Collider2D>();

			Manifold m = collider1->Intersects(collider2);

			if (m.colliding)
			{
				auto emplace = m_colliding.insert(std::make_pair(first, second));

				bool wasEmplaced = emplace.second;

				bool firstIsTrigger = collider1->IsTrigger();
				bool secondIsTrigger = collider2->IsTrigger();

				//OnCollisionEnter
				if (wasEmplaced)
				{
					if (firstIsTrigger || secondIsTrigger)
					{
						first->OnTriggerEnter(second);
						second->OnTriggerEnter(first);
					}
					else
					{
						first->OnCollisionEnter(second);
						second->OnCollisionEnter(first);
					}
				}

				//Prevent non-trigger colliders from intersecting
				if (!firstIsTrigger && !secondIsTrigger)
				{
					collider1->ResolveOverlap(m);
					m.resolve = -m.resolve;
					collider2->ResolveOverlap(m);
				}
			}
		}
	}
}

void CollisionResolver::ProcessCollisionExits()
{
	//Remove objects no longer colliding
	auto itr = m_colliding.begin();
	while (itr != m_colliding.end())
	{
		auto pair = *itr;

		std::shared_ptr<Object> first = pair.first;
		std::shared_ptr<Object> second = pair.second;

		std::shared_ptr<C_Collider2D> c1 = first->GetComponent<C_Collider2D>();
		std::shared_ptr<C_Collider2D> c2 = second->GetComponent<C_Collider2D>();

		bool firstIsTrigger = c1->IsTrigger();
		bool secondIsTrigger = c2->IsTrigger();

		Manifold m = c1->Intersects(c2);

		if (!m.colliding)
		{
			//OnCollisionExit

			if (firstIsTrigger || secondIsTrigger)
			{
				first->OnTriggerExit(second);
				second->OnTriggerExit(first);
			}
			else
			{
				first->OnCollisionExit(second);
				second->OnCollisionExit(first);
			}
	
			itr = m_colliding.erase(itr);
		}
		else
		{
			//OnCollisionStay

			if (firstIsTrigger || secondIsTrigger)
			{
				first->OnTriggerStay(second);
				second->OnTriggerStay(first);
			}
			else
			{
				//TODO: if all non-trigger collisions are resolved will this ever be called?
				first->OnCollisionStay(second);
				second->OnCollisionStay(first);
			}

			++itr;
		}
	}
}

void CollisionResolver::ProcessCollisionExitDueToRemovals()
{
	//Remove objects no longer colliding
	auto itr = m_colliding.begin();
	while (itr != m_colliding.end())
	{
		auto pair = *itr;

		std::shared_ptr<Object> first = pair.first;
		std::shared_ptr<Object> second = pair.second;

		if (first->IsQueuedForRemoval() || second->IsQueuedForRemoval())
		{

			bool firstIsTrigger = first->GetComponent<C_Collider2D>()->IsTrigger();
			bool secondIsTrigger = second->GetComponent<C_Collider2D>()->IsTrigger();

			//OnCollisionExit
			if (firstIsTrigger || secondIsTrigger)
			{
				first->OnTriggerExit(second);
				second->OnTriggerExit(first);
			}
			else
			{
				first->OnCollisionExit(second);
				second->OnCollisionExit(first);
			}

			itr = m_colliding.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}


