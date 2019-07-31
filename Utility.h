#pragma once

#include "SFML\Graphics.hpp"

void centerOrigin(sf::Sprite& m_sprite, sf::Vector2f offset = sf::Vector2f(0, 0));

sf::Vector2f normalize(sf::Vector2f vector);
float norm(sf::Vector2f vector);

float distance(sf::Vector2f a, sf::Vector2f b);

extern int counter;