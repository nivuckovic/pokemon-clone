#pragma once

#include <string>

class CharacterCoreIO
{
public:
	std::string hashFile(const std::string& input) const;

protected:
	const char* HASH = "hash";

	const char* LOCATION_ID = "location.id";
	const char* MAP_POSITION = "map.position";
};

