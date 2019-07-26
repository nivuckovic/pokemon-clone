#pragma once
#include "Entity.h"

#include "Category.h"
#include "ResourceIdentifier.h"
#include "AnimatedSprite.h"

class Human : public Entity {
public:
	enum Type {
		Player,
		NPC
	};

public:
	Human(Type type, const TextureHolder& textures);

	unsigned int getCategory() const override;

	sf::FloatRect getBoundingBox() const override;

protected:
	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

	void updateCurrent(sf::Time dt) override;

private:
	Type m_type;

	AnimatedSprite m_animatedSprite;
	std::unique_ptr<Animation> m_walkingAnimations[4];

	sf::FloatRect m_boundingRect;
};

