#pragma once

#include "LocationID.h"

struct CharacterCoreData {
	bool hashValid;

	Location::ID m_currentLocation;
	sf::Vector2f m_currentMapPosition;

};