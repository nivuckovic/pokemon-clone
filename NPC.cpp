#include "NPC.h"

#include "ResourceHolder.h"

NPC::NPC(Category::Type category, const TextureHolder& textures) : m_directionIndex(0), m_category(category)
{
	m_directions.push_back(MovementDirection(Directions::Right, 32.f));
	m_directions.push_back(MovementDirection(Directions::Down, 32.f));
	m_directions.push_back(MovementDirection(Directions::Left, 32.f));
	m_directions.push_back(MovementDirection(Directions::Top, 32.f));

	Entity::setSpeed(50.f);

	for (int i = 0; i < Directions::Count; ++i) {
		std::unique_ptr<Animation> animation(new Animation(textures.get(Textures::Player), i, 3, 16, 24));
		m_walkingAnimations[i] = std::move(animation);
	}

	m_animatedSprite.setAnimation(*m_walkingAnimations[Directions::Down].get());
}

void NPC::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_animatedSprite, states);
}

void NPC::updateCurrent(sf::Time dt)
{
	Entity::updateCurrent(dt);

	updateMovementPattern(dt);

	m_animatedSprite.setAnimation(*m_walkingAnimations[Entity::getDirection()]);

	if (getMoving()) {
		m_animatedSprite.update(dt);
	}
}

unsigned int NPC::getCategory() const
{
	return m_category;
}

sf::FloatRect NPC::getBoundingBox() const
{
	sf::FloatRect bounds = m_animatedSprite.getLocalBounds();
	return getWorldTransform().transformRect(sf::FloatRect(bounds.left, bounds.top + 8.f, bounds.width, bounds.height - 8.f));
}

void NPC::updateMovementPattern(sf::Time dt)
{

	if (!m_directions.empty()) {
		float distanceToTravel = m_directions[m_directionIndex].m_distance;

		if (m_travelledDistance >= distanceToTravel) {
			m_directionIndex = (m_directionIndex + 1) % m_directions.size();
			m_travelledDistance = 0;
		}

		sf::Vector2f direction;
		switch (m_directions[m_directionIndex].m_direction)
		{
		case Directions::Left:
			direction = sf::Vector2f(-1 * 16, 0);
			break;

		case Directions::Right:
			direction = sf::Vector2f(1 * 16, 0);
			break;

		case Directions::Down:
			direction = sf::Vector2f(0, 1 * 16);
			break;

		case Directions::Top:
			direction = sf::Vector2f(0, -1 * 16);
			break;
		}

		setMoveDirection(direction, m_directions[m_directionIndex].m_direction);

	}
}
