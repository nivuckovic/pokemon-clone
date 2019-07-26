#pragma once

#include <string>

enum Directions {
	Down,
	Top,
	Left,
	Right,
	Count
};

Directions getDirection(std::string& direction);