#pragma once

#include <map>
#include "ResourceIdentifier.h"

namespace Location {
	enum ID {
		None,
		PalletTown,
		Route5,
		Route4,
		Route9,
		Route24,
		CeruleanCity,
		PokemonCenter,
		PokemonMarket,
		PokemonHouse1,
		LocationCount
	};

	Textures::ID toTextureID(Location::ID id);

	extern std::map <ID, std::string> m_filenames;
}