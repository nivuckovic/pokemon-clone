#include "TileMapNode.h"

#include "ResourceHolder.h"
#include "LocationID.h"

TileMapNode::TileMapNode(TextureHolder & textures, LevelData & levelData, int layer) : m_category(Category::Type::Background)
{
	m_locationID = levelData.m_locationID;

	m_tileset = textures.get(Location::toTextureID(levelData.m_locationID));

	int width = levelData.m_mapSize.x;
	int height = levelData.m_mapSize.y;
	
	m_size = sf::FloatRect(0, 0, width * 16.f, height * 16.f);

	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(width * height * 4);

	std::vector<int>& tiles = levelData.m_backgroundTileLayer[layer];
	for (int i = 0; i < width; ++i){
		for (int j = 0; j < height; ++j) {
			int tileNumber = tiles[i + j * width];
			//jer tile krece od 1 mi zelimo od 0
			--tileNumber;

			int tu = tileNumber % width;
			int tv = std::floor(tileNumber / width);

			sf::Vertex * quad = &m_vertices[(i + j * width) * 4];

			sf::Vector2i tileSize = levelData.m_tileSize;

			quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
			quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
			quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
			quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

			quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
			quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
			quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
			quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
		}
	}
}

sf::FloatRect TileMapNode::getBoundingBox() const
{
	return getWorldTransform().transformRect(m_size);
}

unsigned int TileMapNode::getCategory() const
{
	return m_category;
}

unsigned int TileMapNode::getLocationID() const
{
	return m_locationID;
}

void TileMapNode::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.texture = &m_tileset;

	target.draw(m_vertices, states);
}
