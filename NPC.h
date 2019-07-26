#pragma once

#include "InteractableTile.h"
#include "Entity.h"

#include "AnimatedSprite.h"
#include "MovementTable.h"

class NPC : public InteractableTile, public Entity{
public:
	NPC(Category::Type category, const TextureHolder& textures);
	
protected:
	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

	void updateCurrent(sf::Time dt) override;

	unsigned int getCategory() const override;
	sf::FloatRect getBoundingBox() const override;

private:
	void updateMovementPattern(sf::Time dt);

private:
	AnimatedSprite m_animatedSprite;
	std::unique_ptr<Animation> m_walkingAnimations[4];

	int m_directionIndex;
	Category::Type m_category;

	std::vector<MovementDirection> m_directions;
};

