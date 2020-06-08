#ifndef MAPEDITORMODE_H
#define MAPEDITORMODE_H
#include "GameItself\game.h"

std::vector<Tile> mapeditor(sf::Texture &texture, std::vector<Tile> &MAP);
void saveMap();
std::vector<Tile> loadMap();
#endif // MAPEDITORMODE_H
