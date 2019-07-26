#pragma once
#include "CollidableTile.h"
#include "Category.h"
#include "LevelData.h"

class EntranceNode : public CollidableTile
{
public:
	EntranceNode(Category::Type category);

	void setInfo(EntranceInfo & m_info);
	
	Location::ID getLocation();
	sf::Vector2f getPosition();

private:
	Location::ID m_location;
	sf::Vector2f m_position;
};

