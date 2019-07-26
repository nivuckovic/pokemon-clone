#pragma once
#include "SceneNode.h"

#include "LocationID.h"

class CollidableTile : public SceneNode
{
public:
	CollidableTile(Category::Type category = Category::Collidable);

	unsigned int getCategory() const override;

	void setBoundingBox(sf::FloatRect & box);

	sf::FloatRect getBoundingBox() const override;

private:
	void updateCurrent(sf::Time dt) override;

	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	Category::Type m_type;
	sf::FloatRect m_boundingBox;

};

