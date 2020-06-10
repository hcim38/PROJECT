#ifndef MAPEDITORMODE_H
#define MAPEDITORMODE_H
#include "GUI\mapedittools.h"
#include "GameItself\game.h"

void mapeditor(Game &game);
void saveMap();
std::vector<Tile> loadMap();
#endif // MAPEDITORMODE_H
