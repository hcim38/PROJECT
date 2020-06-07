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
    sf::Text textValue;

public:
    bool offset = 0;

    Tile(sf::Texture &texture, sf::Vector2i size, sf::Vector2f pos);
    Tile(bool Null);

    friend void plus1ForEveryone(std::vector<Tile> &tiles);
    friend std::vector<Tile> generateTemplate(sf::Texture &m_textures,
                                              sf::Vector2i tileSize,
                                              unsigned int mapSize);

    int value() const { return m_value; }
    sf::Vector2i tilesize() { return m_tilesize; }
    bool origin() { return m_origin; }
    bool Null() { return m_Null; }
    void makeOrigin() { m_origin = true; }
    void remOrigin() { m_origin = false; }

    void setBegginerValue() { m_value = 2; } //TOOD make protected

    void valPlus1(unsigned long long &pointsLeft); //TODO make protected
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

    void drawMe(sf::RenderTarget &window, sf::Font &font)
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
};

#endif // TILE_H
