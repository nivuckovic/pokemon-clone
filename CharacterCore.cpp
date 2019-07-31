#include "CharacterCore.h"

#include "CharacterCoreReader.h"
#include "CharacterCoreWriter.h"

void CharacterCore::loadNew()
{
	m_data.m_currentLocation = Location::CeruleanCity;
	m_data.m_currentMapPosition = sf::Vector2f(43.f, 18.f);
}

bool CharacterCore::load(const std::string & fileName)
{
	CharacterCoreReader reader;

	if (!reader.readCharacterCore(fileName, m_data)) {
		return false;
	}

	return true;
}

bool CharacterCore::save(const std::string & fileName, const std::string & name)
{
	CharacterCoreWriter writer;

	return writer.saveToFile(fileName, m_data);
}

const CharacterCoreData & CharacterCore::getData() const
{
	return m_data;
}

void CharacterCore::setMap(const sf::Vector2f & position, const Location::ID locationID)
{
	m_data.m_currentLocation = locationID;
	m_data.m_currentMapPosition = position;
}
