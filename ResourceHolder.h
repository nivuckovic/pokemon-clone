#pragma once

#include <map>
#include <memory>
#include <string>
#include <cassert>

template <typename Identifier, typename Resource>
class ResourceHolder {

public:
	void load(Identifier identifier, const std::string& filename);

	Resource& get(Identifier id);
	Resource& get(Identifier id) const;
private:
	void insertResource(Identifier id, std::unique_ptr<Resource> resource);

private:
	std::map<Identifier, std::unique_ptr<Resource>> m_resourceMap;

};















template<typename Identifier, typename Resource>
void ResourceHolder<Identifier, Resource>::load(Identifier identifier, const std::string & filename)
{
	std::unique_ptr<Resource> resource(new Resource());

	if (!resource->loadFromFile(filename)) {
		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);
	}

	insertResource(identifier, std::move(resource));
}

template<typename Identifier, typename Resource>
Resource & ResourceHolder<Identifier, Resource>::get(Identifier id)
{
	auto found = m_resourceMap.find(id);
	assert(found != m_resourceMap.end());

	return *found->second;
}

template<typename Identifier, typename Resource>
Resource & ResourceHolder<Identifier, Resource>::get(Identifier id) const
{
	auto found = m_resourceMap.find(id);
	assert(found != m_resourceMap.end());

	return *found->second;
}

template<typename Identifier, typename Resource>
void ResourceHolder<Identifier, Resource>::insertResource(Identifier id, std::unique_ptr<Resource> resource)
{
	auto inserted = m_resourceMap.insert(std::make_pair(id, std::move(resource)));
	assert(inserted.second);
}
