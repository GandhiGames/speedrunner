#pragma once

#include <map>
#include <memory>

//TODO: all resources stick around even if no longer required. Add a count for resource requests/releases, when reaches zero remove resource.
//TODO: currently limited to classes that have method loadFromFile(filePath), need to extend?
template<typename T>
class ResourceManager
{
public:

	// Adds a resource to the manager, and returns its id in the map.
	int Add(std::string filePath)
	{
		// First check if the resource has already been created. If so, simply return that one.
		auto it = m_resources.find(filePath);

		if (it != m_resources.end())
		{
			return it->second.first;
		}

		// At this point the texture doesn't exists, so we'll create and add it.

		std::shared_ptr<T> resource = std::make_shared<T>();
		if (!resource->loadFromFile(filePath))
		{
			return -1;
		}

		m_resources.insert(std::make_pair(filePath, std::make_pair(m_currentId, resource)));

		// Return the resource id.
		return m_currentId++;
	}

	// Removes a texture from the manager from a given id.
	void Remove(int id)
	{
		for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
		{
			if (it->second.first == id)
			{
				m_resources.erase(it->first);
			}
		}
	}

	// Gets a texture from the texture manager from an ID.
	std::shared_ptr<T> Get(int id)
	{
		for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
		{
			if (it->second.first == id)
			{
				return it->second.second;
			}
		}

		return nullptr;
	}

	bool Has(int id)
	{
		return (Get(id) != nullptr);
	}

private:
	int m_currentId;
	std::map<std::string, std::pair<int, std::shared_ptr<T>>> m_resources;
};

