#ifndef PLAYER_H
#define PLAYER_H
#include "tile.h"

class Player
{
private:
    bool m_AI;
    std::string m_nickname;
    sf::Color m_playersColor;
    std::vector<Tile> m_ownership;

public:
    friend std::vector<Player> setupPlayers(std::vector<Tile> &map); //FIXME TEMP
    friend int main();
    friend void Turnmanager(std::vector<Player> &players, Tile &clickedAt, unsigned long long &turn);
    friend bool addPointsToTiles(Tile &clickedAt, Player &player, unsigned long long &pointsLeft);
    friend void AI(std::vector<Player> &players, unsigned long long &turn);

    Player(std::string nickname, int posInVec, bool AI = 0)
    {
        m_AI = AI;
        m_nickname = nickname;
        switch (posInVec) {
        case 0:
            m_playersColor = sf::Color::Blue;
            break;
        case 1:
            m_playersColor = sf::Color::Green;
            break;
        case 2:
            m_playersColor = sf::Color::Red;
            break;
        case 3:
            m_playersColor = sf::Color::Magenta;
            break;
        case 4:
            m_playersColor = sf::Color::Yellow;
            break;
        default:
            m_playersColor = sf::Color::Black;
            break;
        }
    }
    Player(std::vector<Tile> map)
    {
        m_nickname = "MAP";
        m_playersColor = sf::Color(255, 255, 255, 255);
        for (auto val : map) {
            m_ownership.emplace_back(val);
        }
    }

    std::vector<Tile> ownership() { return m_ownership; }
    sf::Color playersColor() { return m_playersColor; }
    bool AI() { return m_AI; }
    std::string nickname() { return m_nickname; }

    void addTileOwnership(Tile target) { m_ownership.emplace_back(target); }
    void removeOwnership(Tile target)
    {
        for (auto it = m_ownership.begin(); it != m_ownership.end(); it++)
            if (*it == target) {
                m_ownership.erase(it);
                break;
            }
    }

    void colorCorrection();
    void textCorrection();
};

#endif // PLAYER_H
