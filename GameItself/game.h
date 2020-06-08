#ifndef GAME_H
#define GAME_H
#include "GameItself\banner.h"

class Game
{
public:
    Game();
    void clicked(sf::Vector2i pos, std::vector<Player> &players, Tile &clickedAt);
    void capture(Tile target, Player &loser, Player &winner);
    void AI(std::vector<Player> &players, unsigned long long &turn);
    void Turnmanager(std::vector<Player> &players, Tile &clickedAt, unsigned long long &turn);
    bool addPointsToTiles(Tile &clickedAt, Player &player, unsigned long long &pointsLeft);
    void plus1ForEveryone(std::vector<Tile> &tiles);
    std::vector<Tile> generateTemplate(sf::Texture &m_textures,
                                       sf::Vector2i tileSize,
                                       unsigned int mapSize);
    std::vector<Player> setupPlayers(std::vector<Tile> &map,
                                     int playersInGame = 4,
                                     int AIplayersInGame = 3);
    std::vector<sf::VertexArray> createLines(std::vector<Player> &players);
    void manualConfig(std::vector<Tile> &map, std::vector<Player> &players);
    void nextTurn(unsigned long long &turn, std::vector<Player> &players);
    void hilightOrigin(Player &player);

    //vars

    Banner banner;

    std::vector<Tile> MAP;
    std::vector<Player> players;
    std::vector<sf::VertexArray> Lines;

    unsigned long long turn = 1, pointsLeft = 0;
    bool pointsGiveAway = 0;
    unsigned int winCondition = 0, TilesOnScreen = 0;

    sf::Clock clock;
    sf::Time time;
    Tile clickedAt;

    QResource *qrTexturePtr;
    QResource *qrFontPtr;

    sf::Texture texture;
    sf::Font font;
};

#endif // GAME_H
