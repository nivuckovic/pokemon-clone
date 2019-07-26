#include "CharacterCoreIO.h"

std::string CharacterCoreIO::hashFile(const std::string & input) const
{
	size_t hash = std::hash<std::string>()(input);

	return std::to_string(hash);
}
