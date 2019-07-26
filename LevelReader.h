#pragma once

#include "tinyxml2.h"
#include "LevelData.h"
#include <assert.h>

class LevelReader {
public:
	bool readWorld(const std::string & fileName, LevelData & levelData, bool read = false);

private:
	bool readMapProperties(tinyxml2::XMLElement * map, LevelData & levelData);
	bool readLayers(tinyxml2::XMLElement * map, LevelData & levelData);
	bool readLayer(const std::string & layer, std::vector<int> & container);
	bool readBoxes(const std::string & fileName, LevelData & levelData);

	void readBox(tinyxml2::XMLNode* root, tinyxml2::XMLElement* box, std::vector<sf::FloatRect>* container);
	void readEntranceBox(tinyxml2::XMLNode* root, tinyxml2::XMLElement* box, std::vector<sf::FloatRect>* container, std::vector<EntranceInfo>* infoContainer);
};

