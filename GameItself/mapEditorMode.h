#ifndef MAPEDITORMODE_H
#define MAPEDITORMODE_H
#include "GameItself\game.h"

class MapEditor : public Game
{
public:
    MapEditor();

    std::vector<Tile> deletedTilesMapEdit;
    bool killTheMapEdit = 0;
    bool newTemplateMapEdit = 0;

    void mapeditor();
    void saveMap();
    std::vector<Tile> loadMap();
    std::vector<sf::VertexArray> createLines(std::vector<Tile> &tiles);
    bool clicked(sf::Vector2i pos, std::vector<Tile> &tiles, Tile &clickedAt);
};
#endif // MAPEDITORMODE_H
