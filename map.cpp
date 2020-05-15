#include "map.h"

bool TileMap::load(const std::string &textures,
                   sf::Vector2i tileSize,
                   unsigned int mapSize) //ladowanie mapy
{
    if (!m_textures.loadFromFile(textures)) //ladownie textur
        return false;

    for (unsigned int i = 0; i < mapSize; ++i)
        for (unsigned int j = 0; j < mapSize;
             ++j) { //petla wypelniajaca vector do ustalonych rozmiarow
            Tile temp(m_textures,
                      tileSize,
                      sf::Vector2f(i * tileSize.x * 2 + 16, 2 * j * tileSize.y + 16));
            temp.m_tilesize = tileSize;           //ustalenie rozmiaru obiektu
            temp.m_position = sf::Vector2i(i, j); //ustalenie pozycji
            m_objects.emplace_back(temp);
        }

    return true;
}

void TileMap::updateMap(std::vector<Tile> &playerOwnership) //FIXME USELESS funckcja
{
    for (auto &val : m_objects) {
        for (auto &val2 : playerOwnership) {
            if (val == val2)
                val = val2;
        }
    }
}
