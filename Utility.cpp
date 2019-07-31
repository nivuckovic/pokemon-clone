#include "Utility.h"

int counter = 0;

void centerOrigin(sf::Sprite & m_sprite, sf::Vector2f offset)
{
	sf::Vector2f origin((m_sprite.getTexture()->getSize().x + offset.x) / 2 , (m_sprite.getTexture()->getSize().y + offset.y) / 2 );
	m_sprite.setOrigin(origin);
}

sf::Vector2f normalize(sf::Vector2f vector)
{
	float _norm = norm(vector);
	return sf::Vector2f(vector.x / _norm, vector.y / _norm);
}

float norm(sf::Vector2f vector)
{
	return sqrt(vector.x * vector.x + vector.y * vector.y);
}

float distance(sf::Vector2f a, sf::Vector2f b)
{
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}
