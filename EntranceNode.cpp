#include "EntranceNode.h"

EntranceNode::EntranceNode(Category::Type category) : CollidableTile(category)
{
}

void EntranceNode::setInfo(EntranceInfo & m_info)
{
	m_location = m_info.location;
	m_position = m_info.position;
}

Location::ID EntranceNode::getLocation()
{
	return m_location;
}

sf::Vector2f EntranceNode::getPosition()
{
	return m_position;
}

