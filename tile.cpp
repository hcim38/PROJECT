#include "tile.h"

bool Tile::fight(Tile &target)
{
    if (m_value > 1) { //requirement for fight
        if (target.m_value == m_value - 1) {
            //draw
            target.m_value = 1;
            m_value = 1;
            return false;
        } else if (target.m_value < m_value - 1) {
            //won
            target.m_value = m_value - target.m_value - 1;
            m_value = 1;
            return true;
        } else {
            //lost
            target.m_value -= m_value - 1;
            m_value = 1;
            return false;
        }
    }
    return false;
}

void Tile::textCorrection()
{
    setTextureRect(sf::IntRect(value() * m_tilesize.x, 0, m_tilesize.x, m_tilesize.y));
}
