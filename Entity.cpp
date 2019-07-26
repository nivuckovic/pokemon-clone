#include "Entity.h"

#include <math.h>
#include "Utility.h"

Entity::Entity() : m_speed(200),  m_movePosition(0, 0), m_moving(false)
{
}

void Entity::setMoveDirection(sf::Vector2f direction, Directions dir)
{
	if (!m_moving) {
		m_movePosition = getPosition() + direction;
		m_direction = dir;
		m_moving = true;
	}
	
}

void Entity::adoptPosition()
{
	int x = (int)(getPosition().x);
	int y = (int)(getPosition().y);

	int newX = floor(x / 16) * 16 + 8;
	int newY = floor(y / 16) * 16 + 8;

	setPosition(newX, newY);
}

void Entity::stopMoving()
{
	adoptPosition();
	setMoving(false);
	moveDirection = sf::Vector2f(0, 0);
}

void Entity::setMoving(bool moving)
{
	m_moving = moving;
}

bool Entity::getMoving() const
{
	return m_moving;
}

Directions Entity::getDirection()
{
	return m_direction;
}

sf::Vector2f Entity::calculateDirection()
{
	return m_movePosition - getPosition();
}

void Entity::updateCurrent(sf::Time dt)
{
	sf::Vector2f moveDirection(0, 0);
	if (m_moving) {
		moveDirection = normalize(calculateDirection());
		m_travelledDistance += sqrt(moveDirection.x * moveDirection.x + moveDirection.y * moveDirection.y) * m_speed * dt.asSeconds();
	}

	move(moveDirection * m_speed * dt.asSeconds());
	
	if (norm(calculateDirection()) < 1.5f && m_moving) {
		adoptPosition();

		m_travelledDistance += 1.5f;
		m_moving = false;
	}

}

void Entity::setSpeed(float speed)
{
	m_speed = speed;
}

float Entity::getSpeed() const
{
	return m_speed;
}

