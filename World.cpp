#include "World.h"

#include <iostream>

#include "TileMapNode.h"
#include "CollidableTile.h"
#include "WorldNode.h"
#include "EntranceNode.h"
#include "NPC.h"

#include <set>

#include "LocationID.h"
#include "Utility.h"

World::World(sf::RenderWindow & window, CharacterCore** characterCore) : m_window(window), m_worldOffset(50000), m_nextLocationID(Location::ID::None), m_characterCore(characterCore)
{
	loadtextures();
	loadLevel((*m_characterCore)->getData().m_currentLocation);
	buildScene();

	m_worldView.zoom(0.25f);
}

void World::update(sf::Time dt)
{
	while (!m_commandQueue.isEmpty()) {
		m_sceneGraph.onCommand(m_commandQueue.pop(), dt);
	}

	m_sceneGraph.update(dt);
	handleCollision();
}

void World::draw()
{
	m_worldView.setCenter(m_player->getWorldPosition());
	m_window.setView(m_worldView);
	m_window.draw(m_sceneGraph);
}
/*
void World::setLocation(Location::ID locationID)
{
	m_nextLocationID = locationID;

	loadLevel(locationID);
	buildScene();
}
*/
CommandQueue & World::getCommandQueue()
{
	return m_commandQueue;
}

void World::loadtextures()
{
	for (int i = Location::ID::None + 1; i < Location::ID::LocationCount; ++i) {
		m_textureHolder.load(Location::toTextureID((Location::ID) i), Location::m_filenames[(Location::ID) i] + ".png");
	}

	m_textureHolder.load(Textures::Player, "red_walking.png");
}

void World::buildScene()
{
	for (int i = 0; i < Layer::LayerCount; ++i) {
		SceneNode::Ptr layer(new SceneNode);
		m_sceneLayers[i] = layer.get();

		m_sceneGraph.attachChild(std::move(layer));
	}

	buildWorld();

	std::unique_ptr<WorldNode> worldNode(new WorldNode(Category::Type::World, *this));
	m_sceneGraph.attachChild(std::move(worldNode));

	std::unique_ptr<Human> human(new Human(Human::Type::Player, m_textureHolder));
	human->setPosition(m_worldOffset + 8 + 16 * (*m_characterCore)->getData().m_currentMapPosition.x, m_worldOffset + 8 + 16 * (*m_characterCore)->getData().m_currentMapPosition.y);
	m_player = human.get();
	m_sceneLayers[Layer::Objects]->attachChild(std::move(human));

	std::unique_ptr<NPC> npc(new NPC((Category::Type)(Category::NPC | Category::Collidable), m_textureHolder));
	npc->setPosition(m_worldOffset + 8 + 16 * 10, m_worldOffset + 8 + 16 * 13);
	m_sceneLayers[Layer::Objects]->attachChild(std::move(npc));
}

void World::buildWorld()
{
	m_sceneLayers[Layer::Background]->setPosition(m_worldOffset, m_worldOffset);

	//build center node
	std::unique_ptr<TileMapNode> centerTileMap(new TileMapNode(m_textureHolder, m_levelData));
	centerTileMap->setPosition(m_levelOffset);
	
	for (int direction = Directions::Down; direction < Directions::Count; ++direction) {
		if (m_levelData.m_connectingLocations[direction] != Location::ID::None) {
			LevelData map = m_connectingLocations[direction];

			std::unique_ptr<TileMapNode> iTileMap(new TileMapNode(m_textureHolder, map));
			
			float offsetX;
			float offsetY;

			switch (direction)
			{
			case Directions::Down:
				offsetX = m_levelData.m_connectiongLocationsOffset[direction].x * map.m_tileSize.x;
				offsetY = (m_levelData.m_mapSize.y * map.m_tileSize.y);
				break;
				
			case Directions::Left:
				offsetX = -(map.m_mapSize.x * map.m_tileSize.x);
				offsetY = m_levelData.m_connectiongLocationsOffset[direction].y * map.m_tileSize.y;

				break;

			case Directions::Right:
				offsetX = (m_levelData.m_mapSize.x * map.m_tileSize.x);
				offsetY = m_levelData.m_connectiongLocationsOffset[direction].y * map.m_tileSize.y;

				break;

			case Directions::Top:
				offsetX = m_levelData.m_connectiongLocationsOffset[direction].x * map.m_tileSize.x;
				offsetY = -(m_levelData.m_mapSize.y * map.m_tileSize.y);

				break;

			default:
				break;
			}

			iTileMap->setPosition(m_levelOffset.x + offsetX, m_levelOffset.y + offsetY);

			m_sceneLayers[Layer::Background]->attachChild(std::move(iTileMap));
		}

	}
	
	createBoxes(centerTileMap, m_levelData.m_collisionBoxes, Category::Type::Collidable);
	createBoxes(centerTileMap, m_levelData.m_jumpDownBoxes, Category::Type::JumpDown);
	createBoxes(centerTileMap, m_levelData.m_jumpLeftBoxes, Category::Type::JumpLeft);
	createBoxes(centerTileMap, m_levelData.m_jumpRightBoxes, Category::Type::JumpRight);
	createEntrance(centerTileMap, m_levelData.m_entrances, m_levelData.m_entranceInfo, Category::Type::Entrance);
	
	//add center node
	m_sceneLayers[Layer::Background]->attachChild(std::move(centerTileMap));
}



void World::createBoxes(std::unique_ptr<TileMapNode>& node, std::vector<sf::FloatRect>& container, Category::Type category)
{
	for (unsigned int i = 0; i < container.size(); ++i) {
		std::unique_ptr<CollidableTile> iTile(new CollidableTile(category));
		iTile->setBoundingBox(container[i]);
		node->attachChild(std::move(iTile));
	}
}

void World::createEntrance(std::unique_ptr<TileMapNode>& node, std::vector<sf::FloatRect>& container, std::vector<EntranceInfo>& infoContainer, Category::Type category)
{
	for (unsigned int i = 0; i < container.size(); ++i) {
		std::unique_ptr<EntranceNode> iTile(new EntranceNode(category));
		iTile->setBoundingBox(container[i]);
		iTile->setInfo(infoContainer[i]);
		node->attachChild(std::move(iTile));
	}
}


bool matchCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2) {
	unsigned int category1 = colliders.first->getCategory();
	unsigned int category2 = colliders.second->getCategory();

	if (type1 & category1 && type2 & category2) {
		return true;
	}
	else if(type1 & category2 && type2 & category1){
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else {
		return false;
	}
}

void World::handleCollision()
{
	std::set<SceneNode::Pair> collisionPairs;
	m_sceneGraph.checkSceneCollision(m_sceneGraph, collisionPairs);

	for (auto pair : collisionPairs) {
		if (matchCategories(pair, Category::Player, Category::Collidable)) {
			auto& player = static_cast<Human&>(*pair.first);

			player.adoptPosition();
			player.setMoving(false);

			if (pair.second->getCategory() & Category::NPC) {
				auto& npc = static_cast<NPC&>(*pair.second);

				npc.adoptPosition();
				npc.setMoving(false);
			}
		}

		if (matchCategories(pair, Category::NPC, Category::Collidable)) {
			auto& npc = static_cast<NPC&>(*pair.first);

			npc.adoptPosition();
			npc.setMoving(false);
		}

		if (matchCategories(pair, Category::Player, Category::JumpDown)) {
			auto& tileMap = static_cast<TileMapNode&>(*pair.second);

			if (m_player->getDirection() == Directions::Down) {
				Command command;
				command.category = Category::Player;
				command.action = derivedAction<Human>([](Human & human, sf::Time dt) {
					human.setMoveDirection(sf::Vector2f(0, 16), Directions::Down);
				});

				m_commandQueue.push(command);
			}
			else {
				m_player->stopMoving();
			}

		}

		if (matchCategories(pair, Category::Player, Category::JumpLeft)) {
			auto& tileMap = static_cast<TileMapNode&>(*pair.second);

			if (m_player->getDirection() == Directions::Left) {
				Command command;
				command.category = Category::Player;
				command.action = derivedAction<Human>([](Human & human, sf::Time dt) {
					human.setMoveDirection(sf::Vector2f(-16, 0), Directions::Left);
				});

				m_commandQueue.push(command);
			}
			else {
				m_player->stopMoving();
			}

		}

		if (matchCategories(pair, Category::Player, Category::JumpRight)) {
			auto& tileMap = static_cast<TileMapNode&>(*pair.second);

			if (m_player->getDirection() == Directions::Right) {
				Command command;
				command.category = Category::Player;
				command.action = derivedAction<Human>([](Human & human, sf::Time dt) {
					human.setMoveDirection(sf::Vector2f(16, 0), Directions::Right);
				});

				m_commandQueue.push(command);
			}
			else {
				m_player->stopMoving();
			}

		}

		if (matchCategories(pair, Category::Player, Category::Background)) {
			//quick fix
			if (!m_player->getMoving()) {
				auto& tileMap = static_cast<TileMapNode&>(*pair.second);

				for (int i = Directions::Down; i < Directions::Count; ++i) {
					if (tileMap.getLocationID() == m_connectingLocations[i].m_locationID) {

						Location::ID locationID = (Location::ID) tileMap.getLocationID();

						Command command;
						command.category = Category::Type::World;
						command.action = derivedAction<WorldNode>([locationID](WorldNode & node, sf::Time dt) {
							node.changeLocation(locationID);
						});

						m_commandQueue.push(command);

						break;
					}
				}
			}
		}

		if (matchCategories(pair, Category::Player, Category::Entrance)){ 

			if (!m_player->getMoving()) {
				auto& entrance = static_cast<EntranceNode&>(*pair.second);

				sf::Vector2f newPosition(m_worldOffset + 8 + entrance.getPosition().x, m_worldOffset + 8 + entrance.getPosition().y);

				Command command2;
				command2.category = Category::Player;
				command2.action = derivedAction<Human>([newPosition](Human & human, sf::Time dt) {
					human.setPosition(newPosition);
				});

				m_commandQueue.push(command2);

				Location::ID nextLocationID = entrance.getLocation();

				Command command;
				command.category = Category::World;
				command.action = derivedAction<WorldNode>([nextLocationID](WorldNode & node, sf::Time dt) {
					node.changeLocation(nextLocationID);
				});

				m_commandQueue.push(command);
			}
		}
	}
}

void World::loadLevel(Location::ID locationID)
{
	assert(m_levelReader.readWorld(Location::m_filenames[locationID], m_levelData, true));

	//directions
	loadConnectingLevels(m_levelData);
}

void World::loadConnectingLevels(LevelData & levelData)
{
	for (int i = 0; i < Directions::Count; ++i) {
		if (m_levelData.m_connectingLocations[i] != Location::ID::None) {
			m_levelReader.readWorld(Location::m_filenames[m_levelData.m_connectingLocations[i]], m_connectingLocations[i]);
		}
		else {
			m_connectingLocations[i] = LevelData();
		}
	}
}

void World::changeLocation(Location::ID locationID)
{
	if (locationID == Location::ID::None) {
		return;
	}

	if (locationID == m_levelData.m_locationID) {
		return;
	}

	//std::cout << "changed from " << Location::m_filenames[m_levelData.m_locationID] << " to " << Location::m_filenames[locationID] << std::endl;

	bool found = false;
	for (int i = Directions::Down; i < Directions::Count; ++i) {
		if (m_connectingLocations[i].m_locationID == locationID) {
			switch ((Directions) i) {
				case Directions::Right:
					m_levelOffset.x += m_levelData.m_mapSize.x * m_levelData.m_tileSize.x;
					m_levelOffset.y += m_levelData.m_connectiongLocationsOffset[i].y * m_connectingLocations[i].m_tileSize.y;

					break;

				case Directions::Left:
					m_levelOffset.x -= m_connectingLocations[i].m_mapSize.x * m_connectingLocations[i].m_tileSize.x;
					m_levelOffset.y += m_levelData.m_connectiongLocationsOffset[i].y * m_connectingLocations[i].m_tileSize.y;

					break;

				case Directions::Top:
					m_levelOffset.x += m_levelData.m_connectiongLocationsOffset[i].x * m_connectingLocations[i].m_tileSize.x;
					m_levelOffset.y -= m_connectingLocations[i].m_mapSize.y * m_connectingLocations[i].m_tileSize.y;

					break;

				case Directions::Down:
					m_levelOffset.x += m_levelData.m_connectiongLocationsOffset[i].x * m_connectingLocations[i].m_tileSize.x;
					m_levelOffset.y += m_levelData.m_mapSize.y * m_levelData.m_tileSize.y;

					break;
			}

			found = true;
			break;
		}
	}

	if (!found) {
		m_levelOffset = sf::Vector2f(0, 0);
	}

	m_sceneLayers[Layer::Background]->detachChildren();
	loadLevel(locationID);
	buildWorld();
}

unsigned int World::oppositeDirection(unsigned int direction)
{
	switch ((Directions)direction)
	{
	case Directions::Down: return Directions::Top; break;
	case Directions::Top: return Directions::Down; break;
	case Directions::Left: return Directions::Right; break;
	case Directions::Right: return Directions::Left; break;

	default:
		break;
	}
}

sf::Vector2f World::getPlayerPosition()
{
	sf::Vector2f position = m_player->getWorldPosition();
	
	position.x = (position.x - m_worldOffset - 8 - m_levelOffset.x) / 16.f;
	position.y = (position.y - m_worldOffset - 8 - m_levelOffset.y) / 16.f;

	return position;
}

Location::ID World::getLocationID()
{
	return m_levelData.m_locationID;
}

