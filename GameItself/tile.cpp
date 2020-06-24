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
    p_Null = 1;
    setColor(sf::Color(255, 255, 255, 0));
    p_position = sf::Vector2i(-10, -10);
}

int Tile::value() const
{
    return p_value;
}

sf::Vector2i Tile::tilesize()
{
    return p_tilesize;
}

sf::Vector2i Tile::position()
{
    return p_position;
}

bool Tile::origin()
{
    return p_origin;
}

bool Tile::Null()
{
    return p_Null;
}

void Tile::makeOrigin()
{
    p_origin = true;
}

void Tile::remOrigin()
{
    p_origin = false;
}

void Tile::setBegginerValue()
{
    p_value = 2;
}

void Tile::valPlus1(unsigned long long &pointsLeft)
{
    if (p_value < 12) {
        p_value += 1;
        pointsLeft -= 1;
    }
}

void Tile::swapOrigin(Tile &newOrigin)
{
    p_origin = false;
    newOrigin.p_origin = true;
}

bool Tile::fight(Tile &target)
{
    if (p_value > 1) { //requirement for fight
        if (target.p_value == p_value - 1) {
            //draw
            target.p_value = 1;
            p_value = 1;
            return false;
        } else if (target.p_value < p_value - 1) {
            //won
            target.p_value = p_value - target.p_value - 1;
            p_value = 1;
            return true;
        } else {
            //lost
            target.p_value -= p_value - 1;
            p_value = 1;
            return false;
        }
    }
    return false;
}

void Tile::textCorrection()
{
    setTextureRect(sf::IntRect(value() * p_tilesize.x, 0, p_tilesize.x, p_tilesize.y));
}

bool Tile::operator==(const Tile &rhs) const
{
    if (p_position.x == rhs.p_position.x && p_position.y == rhs.p_position.y)
        return true;
    else
        return false;
}

bool Tile::operator==(const sf::Vector2i &rhs) const
{
    if (p_position.x == rhs.x && p_position.y == rhs.y)
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
    if (p_position.y == target.p_position.y) {
        return (p_position.x == target.p_position.x + 1)
               || (p_position.x == target.p_position.x - 1);
    }
    if (!p_offset) {
        if (p_position.y == target.p_position.y + 1) {
            return (p_position.x == target.p_position.x + 1)
                   || (p_position.x == target.p_position.x);
        }

        if (p_position.y == target.p_position.y - 1) {
            return (p_position.x == target.p_position.x + 1)
                   || (p_position.x == target.p_position.x);
        }
    } else {
        if (p_position.y == target.p_position.y + 1) {
            return (p_position.x == target.p_position.x - 1)
                   || (p_position.x == target.p_position.x);
        }

        if (p_position.y == target.p_position.y - 1) {
            return (p_position.x == target.p_position.x - 1)
                   || (p_position.x == target.p_position.x);
        }
    }
    return false;
}

void Tile::drawMe(sf::RenderTarget &window, sf::Font &font)
{
    window.draw(*this);

    textValue.setFont(font);
    if (p_value > 0) {
        textValue.setString(std::to_string(p_value));

        textValue.setPosition(getPosition());
        textValue.move(11, 7);
        if (p_value > 9) {
            textValue.move(-3, 0);
        }
        window.draw(textValue);
    }
}

void Tile::setUpTile(sf::Vector2i &tilesize, sf::Vector2i position, bool offset)
{
    p_tilesize = tilesize;
    p_position = position;
    p_offset = offset;
}
