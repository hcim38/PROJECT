#ifndef MAINFUNCTIONS_H
#define MAINFUNCTIONS_H

#include "player.h"
#include <QRandomGenerator>
void clicked(sf::Vector2i pos, std::vector<Player> &players, Tile &clickedAt);
void capture(Tile target, Player &loser, Player &winner);
bool sortByScore(const Tile &one, const Tile &two);
void AI(std::vector<Player> &players, unsigned long long &turn);
void Turnmanager(std::vector<Player> &players, Tile &clickedAt, unsigned long long &turn);
bool addPointsToTiles(Tile &clickedAt, Player &player, unsigned long long &pointsLeft);
void plus1ForEveryone(std::vector<Tile> &tiles);
std::vector<Tile> loadMap(sf::Texture &m_textures, sf::Vector2i tileSize, unsigned int mapSize);
std::vector<Player> setupPlayers(std::vector<Tile> &map,
                                 int playersInGame = 4,
                                 int AIplayersInGame = 3);
#endif // MAINFUNCTIONS_H
