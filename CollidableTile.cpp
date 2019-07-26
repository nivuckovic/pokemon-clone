#include "CollidableTile.h"

CollidableTile::CollidableTile(Category::Type category) : m_type(category)
{
}

unsigned int CollidableTile::getCategory() const
{
	return m_type;
}

void CollidableTile::setBoundingBox(sf::FloatRect & box)
{
	m_boundingBox = box;
}

sf::FloatRect CollidableTile::getBoundingBox() const
{
	return getWorldTransform().transformRect(m_boundingBox);
}

void CollidableTile::updateCurrent(sf::Time dt)
{
}

void CollidableTile::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{

}
