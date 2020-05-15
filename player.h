#ifndef PLAYER_H
#define PLAYER_H
#include "map.h"

class Player //NOTE Gracz i zestaw jego funkcji
{
public:
    std::vector<Tile> m_ownership;
    bool m_AI;

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
    Player(TileMap map)
    {
        m_nickname = "MAP";
        m_playersColor = sf::Color(255, 255, 255, 255);
        for (auto val : map.m_objects) {
            m_ownership.emplace_back(val);
        }
    }
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

    std::string m_nickname;
    sf::Color m_playersColor;
};

#endif // PLAYER_H
