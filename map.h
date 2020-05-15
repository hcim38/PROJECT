#ifndef MAP_H
#define MAP_H
#include "tile.h"

class TileMap //Generacja mapy //TODO finetuning
{
public:
    sf::VertexArray m_vertices;
    sf::Texture m_textures;
    std::vector<Tile> m_objects;

    bool load(const std::string &textures, sf::Vector2i tileSize, unsigned int mapSize);
    void updateMap(std::vector<Tile> &playerOwnership);
};

#endif // MAP_H
