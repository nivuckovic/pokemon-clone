#pragma once

#include "Directions.h"

struct MovementDirection {
	MovementDirection(Directions direction, float distance) : m_direction(direction), m_distance(distance) {};

	Directions m_direction;
	float m_distance;
};