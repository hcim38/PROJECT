#ifndef GAME_H
#define GAME_H
#include "GameItself/banner.h"
#include "GameItself/functionsOutsideClasses.h"

class Game
{
public:
    Game();
    Game(bool);
    virtual ~Game();

    virtual void clicked(sf::Vector2i pos, std::vector<Player> &players, Tile &clickedAt);
    virtual std::vector<sf::VertexArray> createLines(std::vector<Player> &players);

    void capture(Tile target, Player &loser, Player &winner);
    void generateTemplate(sf::Vector2i tileSize = sf::Vector2(30, 30), unsigned int mapSize = 10);
    void nextTurn(unsigned long long &turn, std::vector<Player> &players);
    void gameLoop();
    void loadMap(QString path, std::vector<Player> &NewPlayers);

    //vars

protected:
    Tile clickedAt;
    std::vector<Tile> MAP;

private:
    QResource *qrTexturePtr;
    QResource *qrFontPtr;
    sf::Texture texture;
    sf::Font font;
    Banner banner;
    std::vector<Player> players;
    std::vector<sf::VertexArray> Lines;
    unsigned long long turn = 1, pointsLeft = 0;
    bool pointsGiveAway = 0;
    unsigned int winCondition = 0, TilesOnScreen = 0, playersEmpty = 0;
};

#endif // GAME_H
