#ifndef PLAYER_H
#define PLAYER_H
#include "tile.h"

class Player
{
private:
    bool p_AI;
    std::string p_nickname;
    sf::Color p_playersColor;
    std::vector<Tile> p_ownership;

public:
    Player(std::string nickname, int posInVec, bool AI = 0)
    {
        p_AI = AI;
        p_nickname = nickname;
        switch (posInVec) {
        case 1:
            p_playersColor = sf::Color::Cyan;
            break;
        case 2:
            p_playersColor = sf::Color::Green;
            break;
        case 3:
            p_playersColor = sf::Color::Red;
            break;
        case 4:
            p_playersColor = sf::Color::Magenta;
            break;
        case 5:
            p_playersColor = sf::Color::Yellow;
            break;
        default:
            p_playersColor = sf::Color::Black;
            break;
        }
    }
    Player(std::vector<Tile> map)
    {
        p_nickname = "MAP";
        p_playersColor = sf::Color(255, 255, 255, 255);
        for (auto val : map) {
            p_ownership.emplace_back(val);
        }
    }

    friend std::vector<Player> setupPlayers(std::vector<Tile> &map,
                                            int playersInGame,
                                            int AIplayersInGame);
    friend int main();
    friend void Turnmanager(std::vector<Player> &players, Tile &clickedAt, unsigned long long &turn);
    friend bool addPointsToTiles(Tile &clickedAt, Player &player, unsigned long long &pointsLeft);
    friend void AI(std::vector<Player> &players, unsigned long long &turn);

    std::vector<Tile> ownership() { return p_ownership; }
    sf::Color playersColor() { return p_playersColor; }
    bool AI() { return p_AI; }
    std::string nickname() { return p_nickname; }

    void addTileOwnership(Tile target) { p_ownership.emplace_back(target); }
    void removeOwnership(Tile target);
    void clearOrigin();

    void colorCorrection();
    void textCorrection();
};

#endif // PLAYER_H
