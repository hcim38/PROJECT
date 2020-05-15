#ifndef TILE_H
#define TILE_H
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <memory>

class Tile : public sf::Sprite //NOTE podstawowy i jedyny obiekt na mapie
{
public:                      //czyszczone po pomyslnym zakonczeniu tury edytowane w turn manager
    sf::Vector2i m_tilesize; //wielkosc i pozycja kloca
    sf::Vector2i m_position;
    bool origin = false;
    bool m_Null = 0;
    int m_value = 0;

    Tile(bool Null)
    {
        if (!Null)
            m_Null = 1;

        m_position = sf::Vector2i(-10, -10);
    }
    Tile(sf::Texture &texture, sf::Vector2i size, sf::Vector2f pos, int value = 0)
    {
        setTexture(texture);
        setPosition(pos);
        setTextureRect(sf::IntRect(0, 0, size.x, size.y));
        m_value = value;
    }
    bool fight(Tile &target);
    bool Null() { return m_Null; }

    void setvalue(int value) { m_value = value; }

    int value() { return m_value; }

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
    //sprawdzanie ruchu: +-1 pion; +- poziom; 0 po skosie;
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
