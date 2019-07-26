#pragma once
#include "CharacterCoreIO.h"

#include "CharacterCoreData.h"

class CharacterCoreWriter : public CharacterCoreIO {
public:
	bool createFile(const std::string& fileName) const;
	bool saveToFile(const std::string& fileName, const CharacterCoreData& data) const;

private:
	std::string writeAndHash(const std::string& in, const CharacterCoreData& data) const;

private:
	std::string writeLocationID(const CharacterCoreData& data) const;
	std::string writeMapPosition(const CharacterCoreData& data) const;
};

