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

int Tile::value() const
{
    return m_value;
}

sf::Vector2i Tile::tilesize()
{
    return m_tilesize;
}

bool Tile::origin()
{
    return m_origin;
}

bool Tile::Null()
{
    return m_Null;
}

void Tile::makeOrigin()
{
    m_origin = true;
}

void Tile::remOrigin()
{
    m_origin = false;
}

void Tile::setBegginerValue()
{
    m_value = 2;
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

bool Tile::operator==(const Tile &rhs) const
{
    if (m_position.x == rhs.m_position.x && m_position.y == rhs.m_position.y)
        return true;
    else
        return false;
}

bool Tile::operator!=(const Tile &rhs) const
{
    return !(*this == rhs);
}

bool Tile::movePossible(Tile &target)
{
    if (m_position.y == target.m_position.y) {
        return (m_position.x == target.m_position.x + 1)
               || (m_position.x == target.m_position.x - 1);
    }
    if (!offset) {
        if (m_position.y == target.m_position.y + 1) {
            return (m_position.x == target.m_position.x + 1)
                   || (m_position.x == target.m_position.x);
        }

        if (m_position.y == target.m_position.y - 1) {
            return (m_position.x == target.m_position.x + 1)
                   || (m_position.x == target.m_position.x);
        }
    } else {
        if (m_position.y == target.m_position.y + 1) {
            return (m_position.x == target.m_position.x - 1)
                   || (m_position.x == target.m_position.x);
        }

        if (m_position.y == target.m_position.y - 1) {
            return (m_position.x == target.m_position.x - 1)
                   || (m_position.x == target.m_position.x);
        }
    }
    return false;
}

void Tile::drawMe(sf::RenderTarget &window, sf::Font &font)
{
    window.draw(*this);

    textValue.setFont(font);
    if (m_value > 0) {
        textValue.setString(std::to_string(m_value));

        textValue.setPosition(getPosition());
        textValue.move(11, 7);
        if (m_value > 9) {
            textValue.move(-3, 0);
        }
        window.draw(textValue);
    }
}
