#include "tile.h"

Tile::Tile(sf::Texture &texture, sf::Vector2i size, sf::Vector2f pos)
{
    setTexture(texture);
    setTextureRect(sf::IntRect(0, 0, size.x, size.y));
    setPosition(pos);
    textValue.setCharacterSize(12);
    setColor(sf::Color(255, 255, 255, 150));
}

Tile::Tile()
{
    m_Null = 1;
    setColor(sf::Color(255, 255, 255, 0));
    m_position = sf::Vector2i(-10, -10);
}

void Tile::valPlus1(unsigned long long &pointsLeft)
{
    if (m_value < 12) {
        m_value += 1;
        pointsLeft -= 1;
    }
}

void Tile::swapOrigin(Tile &newOrigin)
{
    m_origin = false;
    newOrigin.m_origin = true;
}

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
