#pragma once

#include "CharacterCoreData.h"

class CharacterCore
{
public:
	void loadNew();
	bool load(const std::string& fileName);

	bool save(const std::string& fileName, const std::string& name);

	const CharacterCoreData& getData() const;

	void setMap(const sf::Vector2f& position, const Location::ID locationID);

private:
	CharacterCoreData m_data;
};

