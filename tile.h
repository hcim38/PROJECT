#ifndef TILE_H
#define TILE_H
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <QResource>
#include <QtDebug>

class Tile : public sf::Sprite
{
private:
    sf::Vector2i m_tilesize; //wielkosc i pozycja kloca
    sf::Vector2i m_position;
    bool m_origin = false;
    bool m_Null = 0;
    int m_value = 0;

public:
    Tile(sf::Texture &texture, sf::Vector2i size, sf::Vector2f pos, int value = 0)
    {
        setTexture(texture);
        setPosition(pos);
        setTextureRect(sf::IntRect(0, 0, size.x, size.y));
        m_value = value;
    }

    Tile(bool Null)
    {
        if (!Null)
            m_Null = 1;

        m_position = sf::Vector2i(-10, -10);
    }
    friend void plus1ForEveryone(std::vector<Tile> &tiles);
    friend std::vector<Tile> loadMap(sf::Texture &m_textures,
                                     sf::Vector2i tileSize,
                                     unsigned int mapSize);

    int value() const { return m_value; }
    sf::Vector2i tilesize() { return m_tilesize; }
    bool origin() { return m_origin; }
    bool Null() { return m_Null; }
    void makeOrigin() { m_origin = true; }
    void remOrigin() { m_origin = false; }

    void setBegginerValue() { m_value = 2; }

    void valPlus1(unsigned long long &pointsLeft);
    void swapOrigin(Tile &newOrigin);
    bool fight(Tile &target);    
    void textCorrection();

    bool operator!=(const Tile &rhs) const
    {
        if (m_position.x == rhs.m_position.x && m_position.y == rhs.m_position.y)
            return false;
        else
            return true;
    }
    bool operator==(const Tile &rhs) const
    {
        if (m_position.x == rhs.m_position.x && m_position.y == rhs.m_position.y)
            return true;
        else
            return false;
    }
    bool movePossible(Tile &target)
    {
        return (target.m_position.x == m_position.x
                && (target.m_position.y == m_position.y + 1
                    || target.m_position.y == m_position.y - 1))
               || (target.m_position.y == m_position.y
                   && (target.m_position.x == m_position.x + 1
                       || target.m_position.x == m_position.x - 1));
    }
};

#endif // TILE_H
