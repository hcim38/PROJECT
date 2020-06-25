#ifndef PLAYER_H
#define PLAYER_H
#include "GameItself/tile.h"

class Player
{
public:
    Player(QString nickname, int posInVec, bool AI = 0);
    Player(std::vector<Tile> map);

    friend std::vector<Player> setupPlayers(int playersInGame,
                                            int AIplayersInGame,
                                            std::vector<Tile> &MAP);
    friend void Turnmanager(std::vector<Player> &players, Tile &clickedAt, unsigned long long &turn);
    friend void AI(std::vector<Player> &players, unsigned long long &turn);
    friend void captureRandomTiles(std::vector<Tile> &MAP, std::vector<Player> &players);

    std::vector<Tile> ownership();

    sf::Color playersColor();
    sf::Color playersColorH();

    bool AI();
    bool addPointsToTiles(Tile &clickedAt, unsigned long long &pointsLeft);
    void hilightOrigin();

    std::string nickname();

    void capture(Tile target, Player &loser);
    void addTileOwnership(Tile target);
    void removeOwnership(Tile target);
    void clearOrigin();
    void colorCorrection();
    void textCorrection();

    //vars
private:
    bool p_AI;
    QString p_nickname;
    sf::Color p_playersColor;
    std::vector<Tile> p_ownership;
};

#endif // PLAYER_H
