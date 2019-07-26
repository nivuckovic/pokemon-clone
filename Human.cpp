#pragma once

#include "Human.h"

#include "ResourceHolder.h"
#include "Utility.h"

Textures::ID toTextureID(Human::Type type) {

	switch (type)
	{
	case Human::Type::Player:
		return Textures::ID::Player;
		break;
	default:
		return Textures::ID::NPC;
	}
}

Human::Human(Type type, const TextureHolder & textures) : m_type(type) {
	//centerOrigin(m_sprite, sf::Vector2f(0, 8));

	for (int i = 0; i < Directions::Count; ++i) {
		std::unique_ptr<Animation> animation(new Animation(textures.get(toTextureID(type)), i, 3, 16, 24));
		m_walkingAnimations[i] = std::move(animation);
	}

	m_animatedSprite.setAnimation(*m_walkingAnimations[Directions::Down].get());
}

unsigned int Human::getCategory() const
{
	switch (m_type)
	{
	case Type::Player: 
		return Category::Player;
		break;
	default:
		return Category::NPC;
	}
}

sf::FloatRect Human::getBoundingBox() const
{
	sf::FloatRect bounds = m_animatedSprite.getLocalBounds();
	return getWorldTransform().transformRect(sf::FloatRect(bounds.left, bounds.top + 8.f, bounds.width, bounds.height - 8.f));
}

void Human::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_animatedSprite, states);
}

void Human::updateCurrent(sf::Time dt) 
{
	Entity::updateCurrent(dt);

	m_animatedSprite.setAnimation(*m_walkingAnimations[Entity::getDirection()]);

	if (getMoving()) {
		m_animatedSprite.update(dt);
	}

}
