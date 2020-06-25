#ifndef FUNCTIONSOUTSIDECLASSES_H
#define FUNCTIONSOUTSIDECLASSES_H

#include "GameItself/player.h"

bool sortByScore(const Tile &one, const Tile &two);

void AI(std::vector<Player> &players, unsigned long long &turn);

void Turnmanager(std::vector<Player> &players, Tile &clickedAt, unsigned long long &turn);

void captureRandomTiles(std::vector<Tile> &MAP, std::vector<Player> &players);

void plus1ForEveryone(std::vector<Tile> &tiles);

std::vector<Player> setupPlayers(int playersInGame, int AIplayersInGame, std::vector<Tile> &MAP);

#endif // FUNCTIONSOUTSIDECLASSES_H
