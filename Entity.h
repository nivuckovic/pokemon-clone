#pragma once
#include "SceneNode.h"
#include  "Directions.h"

class Entity : public SceneNode {
public:
	Entity();
	void setMoveDirection(sf::Vector2f direction, Directions dir);

	void adoptPosition();
	
	void stopMoving();

	void setMoving(bool moving);

	bool getMoving() const;
	Directions getDirection();

protected:
	void updateCurrent(sf::Time dt) override;

	sf::Vector2f calculateDirection();
	void setSpeed(float speed);
	float getSpeed() const;

	float m_travelledDistance;

private:
	sf::Vector2f m_movePosition;
	sf::Vector2f moveDirection;
	float m_speed;

	bool m_moving;
	Directions m_direction;
};


