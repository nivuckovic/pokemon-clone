#pragma once

#include "LocationID.h"

std::map <Location::ID, std::string> Location::m_filenames = {
	{ Location::None, "none" },
	{ Location::CeruleanCity, "cerulean_city" },
	{ Location::Route4, "route_4" },
	{ Location::Route5, "route_5" },
	{ Location::Route9, "route_9" },
	{ Location::Route24, "route_24" },
	{ Location::PalletTown, "pallet_town"},
	{ Location::PokemonCenter, "pokemon_center" },
	{ Location::PokemonMarket, "pokemon_market" },
	{ Location::PokemonHouse1, "pokemon_house1" },

};

Textures::ID Location::toTextureID(Location::ID id) {
	switch (id) {
	case Location::ID::CeruleanCity:
		return Textures::ID::CeruleanCity;
		break;
	case Location::ID::Route4:
		return Textures::ID::Route4;
		break;
	case Location::ID::Route5:
		return Textures::ID::Route5;
		break;
	case Location::ID::Route9:
		return Textures::ID::Route9;
		break;
	case Location::ID::Route24:
		return Textures::ID::Route24;
		break;
	case Location::ID::PalletTown:
		return Textures::ID::PalletTown;
		break;
	case Location::ID::PokemonCenter:
		return Textures::ID::PokemonCenter;
		break;
	case Location::ID::PokemonMarket:
		return Textures::ID::PokemonMarket;
		break;
	case Location::ID::PokemonHouse1:
		return Textures::ID::PokemonHouse1;
		break;
	default:
		return Textures::ID::Background;
	}
}