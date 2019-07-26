#include "CharacterCoreReader.h"

#include <fstream>
#include <sstream>
#include <assert.h>

#include <iostream>
#include <stdio.h>

static bool readFloatAfterComma(std::string& s, float* f) {
	std::size_t pos = s.find(',');
	
	if (pos == std::string::npos) return false;
	if (s.size() == 1) return false;

	s = s.substr(pos + 1);
	*f = std::stof(s);

	return true;
}

CharacterCoreReader::CharacterCoreReader()
{
	initReadMap();
}

bool CharacterCoreReader::readCharacterCore(const std::string & fileName, CharacterCoreData & data)
{
	std::string line;
	std::ifstream saveFile(fileName);

	assert(saveFile.is_open());

	std::size_t colonPos;
	std::string attribute;

	std::stringstream buffer;
	buffer << saveFile.rdbuf();
	std::string allText = buffer.str();
	allText.erase(0, allText.find("\n") + 1);
	m_correctHash = hashFile(allText);

	saveFile.seekg(0, std::ios::beg);

	while (std::getline(saveFile, line)) {
		colonPos = line.find(':');
		if (colonPos == std::string::npos) continue;

		attribute = line.substr(0, colonPos);

		auto itr = m_readMap.find(attribute);
		assert(itr != m_readMap.end());

		line = line.substr(colonPos + 1);
		
		//kroz mapu iteriramo gledamo attribute za key, dobivamo funkciju koju treba pozvati!!! = GENIJALNO
		if (!((*this).*(itr->second))(line, data)) {
			saveFile.close();
			return false;
		}
	}

	saveFile.close();

	return true;
}

bool CharacterCoreReader::readHash(std::string & line, CharacterCoreData & data) const
{
	data.hashValid = (line == m_correctHash);

	return data.hashValid;
}

bool CharacterCoreReader::readLocationID(std::string & line, CharacterCoreData & data) const
{
	auto found = std::find_if(Location::m_filenames.begin(), Location::m_filenames.end(), [&line](const std::pair<Location::ID, std::string> &pair) { return (pair.second == line); });
	assert(found != Location::m_filenames.end());

	data.m_currentLocation = found->first;

	return true;
}

bool CharacterCoreReader::readMapPosition(std::string & line, CharacterCoreData & data) const
{
	data.m_currentMapPosition.x = std::stof(line);
	assert(readFloatAfterComma(line, &data.m_currentMapPosition.y));

	return true;
}

void CharacterCoreReader::initReadMap()
{
	m_readMap.insert({ HASH, &CharacterCoreReader::readHash });

	m_readMap.insert({ LOCATION_ID, &CharacterCoreReader::readLocationID });
	m_readMap.insert({ MAP_POSITION, &CharacterCoreReader::readMapPosition });
}
