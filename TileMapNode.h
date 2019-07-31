#pragma once
#include "SceneNode.h"

#include "LocationID.h"
#include "ResourceIdentifier.h"
#include "LevelData.h"
#include "Category.h"
#include "LocationID.h"

class TileMapNode : public SceneNode {
public:
	TileMapNode(TextureHolder & textures, LevelData & levelData, int layer = 0);

	sf::FloatRect getBoundingBox() const override;
	unsigned int getCategory() const override;

	unsigned int getLocationID() const;
private:
	void drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const override;

private:
	sf::VertexArray m_vertices;
	sf::Texture m_tileset;

	sf::FloatRect m_size;

	Category::Type m_category;
	Location::ID m_locationID;
};

