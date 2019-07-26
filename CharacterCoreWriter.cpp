#include "CharacterCoreWriter.h"

#include <fstream>

bool CharacterCoreWriter::createFile(const std::string & fileName) const
{
	//promjeni da provjerava je li overwriteas neki file
	//std::ofstream file(fileName);

	return false;
}

bool CharacterCoreWriter::saveToFile(const std::string & fileName, const CharacterCoreData & data) const
{
	std::ofstream saveFile(fileName, std::ios::trunc);

	if (saveFile.is_open()) {
		std::string toHash;

		toHash.append(writeLocationID(data));
		toHash.append(writeMapPosition(data));
	
		saveFile << writeAndHash(toHash, data);
	}
	else {
		return false;
	}

	return true;
}

std::string CharacterCoreWriter::writeAndHash(const std::string & in, const CharacterCoreData & data) const
{
	std::string hash;
	hash.append(std::string(HASH) + ":" + hashFile(in) + "\n");
	return hash + in;
}

std::string CharacterCoreWriter::writeLocationID(const CharacterCoreData & data) const
{
	return std::string(std::string(LOCATION_ID) + ":" + Location::m_filenames[data.m_currentLocation] + "\n");
}

std::string CharacterCoreWriter::writeMapPosition(const CharacterCoreData & data) const
{
	return std::string(std::string(MAP_POSITION) + ":" + std::to_string(data.m_currentMapPosition.x) + ", " + std::to_string(data.m_currentMapPosition.y) + "\n");
}
