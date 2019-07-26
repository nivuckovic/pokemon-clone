#include "LevelReader.h"

#include <iostream>
#include <string>
#include "Directions.h"

std::vector<sf::IntRect> createCollisionBoxes(std::vector<int> & container, LevelData & levelData) {
	std::vector<sf::IntRect> result;

	int width = levelData.m_mapSize.x;
	int height = levelData.m_mapSize.y;

	int tileSize = levelData.m_tileSize.x;

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			if (container[j + i * width]) {
				result.push_back(sf::IntRect(j * tileSize, i * tileSize, tileSize, tileSize));
			}
		}
	}

	return result;
}

bool LevelReader::readWorld(const std::string & fileName, LevelData & levelData, bool read)
{
	auto _result = std::find_if(Location::m_filenames.begin(), Location::m_filenames.end(), [&fileName](const auto& it) { return it.second == fileName; });
	assert(_result != Location::m_filenames.end());
	
	levelData.m_locationID = _result->first;

	tinyxml2::XMLDocument xmlDoc;

	tinyxml2::XMLError result = xmlDoc.LoadFile((fileName + ".tmx").c_str());
	assert(result == tinyxml2::XML_SUCCESS);

	tinyxml2::XMLElement * map = xmlDoc.FirstChildElement("map");
	assert(map != nullptr);

	if (!readMapProperties(map, levelData)) return false;
	if (!readLayers(map, levelData)) return false;
	if (read) {
		if (!readBoxes(fileName, levelData)) return false;
	}

	return true;
}

//ucitamo m_mapSize, m_tileSize, m_tilePath
bool LevelReader::readMapProperties(tinyxml2::XMLElement * map, LevelData & levelData)
{
	const char* textAttr = nullptr;
	textAttr = map->Attribute("renderorder");
	assert(textAttr != nullptr);

	std::string renderorder = textAttr;
	if (renderorder != "right-down") {
		return false;
	}

	textAttr = map->Attribute("orientation");
	assert(textAttr != nullptr);

	std::string orientation = textAttr;
	if (orientation != "orthogonal") {
		return false;
	}

	tinyxml2::XMLError result = map->QueryIntAttribute("width", &levelData.m_mapSize.x);
	assert(result == tinyxml2::XML_SUCCESS);

	result = map->QueryIntAttribute("height", &levelData.m_mapSize.y);
	assert(result == tinyxml2::XML_SUCCESS);

	result = map->QueryIntAttribute("tilewidth", &levelData.m_tileSize.x);
	assert(result == tinyxml2::XML_SUCCESS);

	result = map->QueryIntAttribute("tileheight", &levelData.m_tileSize.y);
	assert(result == tinyxml2::XML_SUCCESS);

	tinyxml2::XMLElement * properties = map->FirstChildElement("properties");
	tinyxml2::XMLElement * property = properties->FirstChildElement("property");

	while (property != nullptr) {
		const char * textAttr = property->Attribute("name");
		std::string name = textAttr;
		textAttr = property->Attribute("value");

		if (name == "down" || name == "top" || name == "left" || name == "right") {
			std::string value;
			value = textAttr;

			auto result = std::find_if(Location::m_filenames.begin(), Location::m_filenames.end(), [value](const auto& it) { return it.second == value; });
			assert(result != Location::m_filenames.end());

			levelData.m_connectingLocations[getDirection(name)] = result->first;
		}

		if(name == "downX" || name == "topX" ){
			float x;
			property->QueryFloatAttribute("value", &x);

			levelData.m_connectiongLocationsOffset[getDirection(name)] = sf::Vector2f(x, 0);
		}

		if (name == "leftY" || name == "rightY") {
			float y;
			property->QueryFloatAttribute("value", &y);

			levelData.m_connectiongLocationsOffset[getDirection(name)] = sf::Vector2f(0, y);
		}

		if(name == "tilesetpath") {
			std::string value;
			value = textAttr;

			textAttr = property->Attribute("value");
			value = textAttr;

			levelData.m_tilePath = value;
		}

		property = property->NextSiblingElement("property");
	}


	return true;
}

bool LevelReader::readLayers(tinyxml2::XMLElement * map, LevelData & levelData)
{
	levelData.m_backgroundTileLayer.clear();

	tinyxml2::XMLElement * layer = map->FirstChildElement("layer");
	while (layer != nullptr) {
		const char * textAttr = layer->Attribute("name");
		std::string name = textAttr;
		
		tinyxml2::XMLElement * data = layer->FirstChildElement("data");
		
		std::string layerData = data->GetText();
		std::vector<int> container;
		
		if (!readLayer(layerData, container)) return false;
		
		if (name == "BG") {
			levelData.m_backgroundTileLayer.push_back(container);
		}

		layer = layer->NextSiblingElement("layer");
	}

	return true;
}

bool LevelReader::readLayer(const std::string & layer, std::vector<int> & container)
{
	std::string layerData = layer;
	
	size_t pos = 0;
	std::vector<int> backgroundLayer;

	while ((pos = layerData.find(",")) != std::string::npos) {
		backgroundLayer.push_back(std::stoi(layerData.substr(0, pos)));
		layerData.erase(0, pos + 1);
	}

	backgroundLayer.push_back(std::stoi(layerData));

	container = backgroundLayer;

	return true;
}

bool LevelReader::readBoxes(const std::string & fileName, LevelData & levelData)
{
	tinyxml2::XMLDocument xmlDoc;

	tinyxml2::XMLError result = xmlDoc.LoadFile((fileName + "_collision.xml").c_str());
	assert(result == tinyxml2::XML_SUCCESS);
	
	tinyxml2::XMLNode* root;
	tinyxml2::XMLElement* box = nullptr;

	readBox(root = xmlDoc.FirstChild(), box, &levelData.m_collisionBoxes);
	readBox(root = root->NextSibling(), box, &levelData.m_jumpDownBoxes);
	readBox(root = root->NextSibling(), box, &levelData.m_jumpLeftBoxes);
	readBox(root = root->NextSibling(), box, &levelData.m_jumpRightBoxes);
	readEntranceBox(root = root->NextSibling(), box, &levelData.m_entrances, &levelData.m_entranceInfo);
	return true;
}

void LevelReader::readBox(tinyxml2::XMLNode * root, tinyxml2::XMLElement * box, std::vector<sf::FloatRect>* container)
{
	container->clear();

	if (root == nullptr)
		return;

	box = root->FirstChildElement("Box");
	while (box != nullptr) {
		sf::FloatRect rect;

		box->QueryFloatAttribute("left", &rect.left);
		box->QueryFloatAttribute("top", &rect.top);
		box->QueryFloatAttribute("width", &rect.width);
		box->QueryFloatAttribute("height", &rect.height);;

		container->push_back(rect);

		box = box->NextSiblingElement("Box");
	}
}

void LevelReader::readEntranceBox(tinyxml2::XMLNode * root, tinyxml2::XMLElement * box, std::vector<sf::FloatRect>* container, std::vector<EntranceInfo>* infoContainer)
{
	container->clear();
	infoContainer->clear();

	if (root == nullptr)
		return;

	box = root->FirstChildElement("Box");
	while (box != nullptr) {
		sf::FloatRect rect;

		box->QueryFloatAttribute("left", &rect.left);
		box->QueryFloatAttribute("top", &rect.top);
		box->QueryFloatAttribute("width", &rect.width);
		box->QueryFloatAttribute("height", &rect.height);;

		EntranceInfo info;

		box->QueryFloatAttribute("X", &info.position.x);
		box->QueryFloatAttribute("Y", &info.position.y);
		const char* location = box->Attribute("location");
		std::string name(location);

		auto found = std::find_if(Location::m_filenames.begin(), Location::m_filenames.end(), [&name](std::pair<Location::ID, std::string> pair) {
			return pair.second == name;
		});
		assert(found != Location::m_filenames.end());

		info.location = found->first;

		container->push_back(rect);
		infoContainer->push_back(info);

		box = box->NextSiblingElement("Box");
	}
}
