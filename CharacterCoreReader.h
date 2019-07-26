#pragma once
#include "CharacterCoreIO.h"
#include "CharacterCoreData.h"

#include <string>
#include <map>

class CharacterCoreReader : public CharacterCoreIO
{
public:
	CharacterCoreReader();

	bool readCharacterCore(const std::string& fileName, CharacterCoreData& data);

private:
	bool readHash(std::string& line, CharacterCoreData& data) const;

	bool readLocationID(std::string& line, CharacterCoreData& data) const;
	bool readMapPosition(std::string& line, CharacterCoreData& data) const;

private:
	std::string m_correctHash;
	//pointer na funkciju clanicu
	typedef bool (CharacterCoreReader::*ReadFunction)(std::string&, CharacterCoreData&) const;

	std::map<std::string, ReadFunction> m_readMap;
	void initReadMap();
};

