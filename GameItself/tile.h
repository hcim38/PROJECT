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
    friend bool sortToFile(Tile const &one, Tile const &two);

    int value() const;
    sf::Vector2i tilesize();
    sf::Vector2i position();
    bool origin();
    bool Null();

    void makeOrigin();
    void remOrigin();
    void setBegginerValue();
    void valPlus1(unsigned long long &pointsLeft);
    void swapOrigin(Tile &newOrigin);     
    void textCorrection();
    void drawMe(sf::RenderTarget &window, sf::Font &font);
    void setUpTile(sf::Vector2i &tilesize, sf::Vector2i position, bool offset);

    bool operator==(const Tile &rhs) const;
    bool operator==(const sf::Vector2i &rhs) const;
    bool operator!=(const Tile &rhs) const;
    bool movePossible(Tile &target);
    bool fight(Tile &target);

    //vars

private:
    sf::Vector2i p_tilesize, p_position;
    bool p_origin = 0, p_Null = 0, p_offset = 0;
    int p_value = 0;
    sf::Text textValue;
};

#endif // TILE_H
