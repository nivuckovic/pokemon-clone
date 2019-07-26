#pragma once

#include "LocationID.h"
#include "SFML\Graphics.hpp"
#include <array>

struct EntranceInfo {
	Location::ID location;
	sf::Vector2f position;
};

struct LevelData {
	Location::ID m_locationID;
	std::string m_tilePath;
	
	std::vector<std::vector<int>> m_backgroundTileLayer;

	std::vector<sf::FloatRect> m_collisionBoxes;
	std::vector<sf::FloatRect> m_jumpDownBoxes;
	std::vector<sf::FloatRect> m_jumpLeftBoxes;
	std::vector<sf::FloatRect> m_jumpRightBoxes;
	std::vector<sf::FloatRect> m_entrances;

	std::vector<EntranceInfo> m_entranceInfo;

	std::array<Location::ID, 4> m_connectingLocations;
	std::array<sf::Vector2f, 4> m_connectiongLocationsOffset;

	sf::Vector2i m_tileSize;
	sf::Vector2i m_mapSize;
};