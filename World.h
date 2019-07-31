#pragma once

#include <SFML\Graphics.hpp>
#include "Human.h"
#include <array>
#include "CommandQueue.h"
#include "LocationID.h"

#include "LevelData.h"
#include "LevelReader.h"

#include "ResourceIdentifier.h"
#include "ResourceHolder.h"
#include "CharacterCore.h"
#include "Directions.h"
#include "TileMapNode.h"


class WorldNode;

class World
{
	friend class WorldNode;

public:
	World(sf::RenderWindow& window, CharacterCore** characterCore);
	
	void update(sf::Time dt);
	void draw();

	CommandQueue& getCommandQueue();
	sf::Vector2f getPlayerPosition();
	Location::ID getLocationID();

private:
	enum Layer {
		Background,
		Objects,
		LayerCount
	};

private:
	void loadtextures();
	void loadLevel(Location::ID locationID);
	void loadConnectingLevels(LevelData & levelData);
	void createBoxes(std::unique_ptr<TileMapNode>& node, std::vector<sf::FloatRect>& container, Category::Type category);
	void createEntrance(std::unique_ptr<TileMapNode>& node, std::vector<sf::FloatRect>& container, std::vector<EntranceInfo>& infoContainer, Category::Type category);

	void buildScene();
	void buildWorld();

	void handleCollision();

	void changeLocation(Location::ID locationID);
	unsigned int oppositeDirection(unsigned int direction);

private:
	sf::RenderWindow& m_window;
	sf::View m_worldView;

	CharacterCore** m_characterCore;

	SceneNode m_sceneGraph;
	std::array<SceneNode*, Layer::LayerCount> m_sceneLayers;
	CommandQueue m_commandQueue;

	Human* m_player;

	LevelData m_levelData;
	LevelData m_connectingLocations[4];
	LevelReader m_levelReader;

	TextureHolder m_textureHolder;

	float m_worldOffset;
	sf::Vector2f m_levelOffset;
	Location::ID m_nextLocationID;
};

