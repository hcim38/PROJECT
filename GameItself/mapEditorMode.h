#ifndef MAPEDITORMODE_H
#define MAPEDITORMODE_H
#include "GameItself/game.h"

class MapEditor : public Game, public QObject
{
public:
    MapEditor();

    void mapeditor();
    void saveMap();
    void loadMap(std::vector<sf::Vector2i> &deleted);

    std::vector<Tile> loadMap();
    std::vector<Tile> getDeleted() { return deletedTilesMapEdit; }

    std::vector<sf::VertexArray> createLines(std::vector<Tile> &tiles);

    bool clicked(sf::Vector2i pos, std::vector<Tile> &tiles);

    void killEditor() { killTheMapEdit = 1; }
    void newTemplate() { newTemplateMapEdit = 1; }

private:
    std::vector<Tile> deletedTilesMapEdit;
    bool newTemplateMapEdit = 0, killTheMapEdit = 0;
};
#endif // MAPEDITORMODE_H
