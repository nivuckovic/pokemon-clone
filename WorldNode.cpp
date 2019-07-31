#include "WorldNode.h"

#include "World.h"

WorldNode::WorldNode(Category::Type category, World& world) : SceneNode(category), m_world(&world)
{
	
}

void WorldNode::changeLocation(Location::ID location)
{
	m_world->changeLocation(location);
}
