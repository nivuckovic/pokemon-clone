#pragma once
#include "SceneNode.h"
#include "LocationID.h"

class World;

class WorldNode : public SceneNode
{

public:
	WorldNode(Category::Type category, World& world);
	
	void changeLocation(Location::ID location);

private:
	World* m_world;
};

