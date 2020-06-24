#ifndef PLAYER_H
#define PLAYER_H
#include "GameItself\tile.h"

class Player
{
public:
    Player(QString nickname, int posInVec, bool AI = 0);
    Player(std::vector<Tile> map);

    friend std::vector<Player> setupPlayers(std::vector<Tile> &map,
                                            int playersInGame,
                                            int AIplayersInGame);
    friend void Turnmanager(std::vector<Player> &players, Tile &clickedAt, unsigned long long &turn);
    friend bool addPointsToTiles(Tile &clickedAt, Player &player, unsigned long long &pointsLeft);
    friend void AI(std::vector<Player> &players, unsigned long long &turn);
    friend void hilightOrigin(Player &player);

    std::vector<Tile> ownership() { return p_ownership; }

    sf::Color playersColor() { return p_playersColor; }
    sf::Color playersColorH();

    bool AI() { return p_AI; }

    std::string nickname() { return p_nickname.toStdString(); }

    void addTileOwnership(Tile target) { p_ownership.emplace_back(target); }
    void removeOwnership(Tile target);
    void clearOrigin();
    void colorCorrection();
    void textCorrection();

    //vars

    bool p_AI;
    QString p_nickname;
    sf::Color p_playersColor;
    std::vector<Tile> p_ownership;
};

#endif // PLAYER_H
