#include "Directions.h"

Directions getDirection(std::string & direction)
{
	if (direction == "down") {
		return Directions::Down;
	}
	if (direction == "top") {
		return Directions::Top;
	}
	if (direction == "left") {
		return Directions::Left;
	}
	if (direction == "right") {
		return Directions::Right;
	}
	if (direction == "downX") {
		return Directions::Down;
	}
	if (direction == "topX") {
		return Directions::Top;
	}
	if (direction == "leftY") {
		return Directions::Left;
	}
	if (direction == "rightY") {
		return Directions::Right;
	}
}
