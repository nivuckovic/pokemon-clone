#pragma once

#include <SFML\Graphics.hpp>

namespace Textures {
	enum ID {
		Player,
		NPC,
		Background,
		CeruleanCity,
		PalletTown,
		Route5,
		Route4,
		Route9,
		Route24,
		PokemonCenter,
		PokemonMarket,
		PokemonHouse1,
	};
}

template <typename Identifier, typename Resource>
class ResourceHolder;

typedef ResourceHolder<Textures::ID, sf::Texture> TextureHolder;