#ifndef TILE_H
#define TILE_H
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <QCoreApplication>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QResource>
#include <QStandardPaths>
#include <QString>
#include <QWidget>

#include "windows.h"

class Tile : public sf::Sprite
{
public:
    Tile(sf::Texture &texture, sf::Vector2i size, sf::Vector2f pos);
    Tile();

    friend void plus1ForEveryone(std::vector<Tile> &tiles);
    friend std::vector<Tile> generateTemplate(sf::Texture &m_textures,
                                              sf::Vector2i tileSize,
                                              unsigned int mapSize);

    int value() const;
    sf::Vector2i tilesize();
    bool origin();
    bool Null();

    void makeOrigin();
    void remOrigin();
    void setBegginerValue();
    void valPlus1(unsigned long long &pointsLeft);
    void swapOrigin(Tile &newOrigin);     
    void textCorrection();
    void drawMe(sf::RenderTarget &window, sf::Font &font);

    bool operator==(const Tile &rhs) const;
    bool operator!=(const Tile &rhs) const;
    bool movePossible(Tile &target);
    bool fight(Tile &target);

    //vars

    sf::Vector2i m_tilesize;
    sf::Vector2i m_position;
    bool m_origin = false;
    bool m_Null = 0;
    int m_value = 0;
    sf::Text textValue;
    bool offset = 0;
};

#endif // TILE_H
