#include "mainFunctions.h"
#include <QResource>
#include <QtDebug>

void duplicatesCheck(std::vector<Player> &Players) //chyba nie znaleziono jeszcze :V
{
    for (auto &PlayerOne : Players) {
        for (auto &tileOne : PlayerOne.ownership()) {
            for (auto &PlayerTwo : Players) {
                if (PlayerOne.playersColor() != PlayerTwo.playersColor()) {
                    for (auto &tileTwo : PlayerTwo.ownership()) {
                        if (tileOne == tileTwo) {
                            qDebug() << "DUPLIKAT KLOCA!!";
                            system("pause");
                        }
                    }
                }
            }
        }
    }
}

std::vector<Tile> loadMap(sf::Texture &m_textures, sf::Vector2i tileSize, unsigned int mapSize)

{
    std::vector<Tile> m_objects;

    for (unsigned int i = 0; i < mapSize; ++i)
        for (unsigned int j = 0; j < mapSize;
             ++j) { //petla wypelniajaca vector do ustalonych rozmiarow
            Tile temp(m_textures,
                      tileSize,
                      sf::Vector2f(i * tileSize.x * 2 + 16, 2 * j * tileSize.y + 16));
            temp.tilesize(tileSize);           //ustalenie rozmiaru obiektu
            temp.position(sf::Vector2i(i, j)); //ustalenie pozycji
            m_objects.emplace_back(temp);
        }

    return m_objects;
}

int main()
{
    //Ustawianie okna gry
    sf::RenderWindow window(sf::VideoMode(640, 640), "Tilemap");
    window.setFramerateLimit(240);
    window.setVerticalSyncEnabled(1);

    QResource qrTexture(":/Textures/tiles.png");
    QResource qrFont(":/Fonts/Lato-Regular.ttf");

    Tile clickedAt(0);
    sf::Texture m_textures;
    sf::Font font;
    std::vector<Tile> MAP;
    sf::RectangleShape indicator(sf::Vector2f(16, 16));
    sf::Text text;

    font.loadFromMemory(qrFont.data(), qrFont.size());
    m_textures.loadFromMemory(qrTexture.data(), qrTexture.size());
    text.setFont(font);
    text.setCharacterSize(8);
    MAP = loadMap(m_textures, sf::Vector2i(32, 32), 10);

    std::vector<Player> players = setupPlayers(MAP);

    unsigned long long turn = 1, pointsLeft = 0;
    bool EndOfTurn = 0, released = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == event.MouseButtonReleased //jezeli kliknieto LMB
                && event.mouseButton.button == sf::Mouse::Left) {
                clicked(sf::Mouse::getPosition(window), //wywyolanie reakcji na kliknecie
                        players,
                        clickedAt);
            }
            if (event.type == event.KeyReleased && event.key.code == sf::Keyboard::Space) {
                if (!players[turn].AI()) {
                    for (auto &player : players) {
                        clearOriginParam(player.m_ownership);
                    }
                    pointsLeft = players[turn].ownership().size();
                    if (EndOfTurn) {
                        released = 1;
                    }
                    EndOfTurn = 1;
                }
            }
        }

        window.clear(sf::Color::Black);

        for (auto &player : players) {
            player.textCorrection();
            player.colorCorrection();
        }

        if (!EndOfTurn) {
            Turnmanager(players, clickedAt, turn);
        } else {
            if (addPointsToTiles(clickedAt, players[turn], pointsLeft) || (EndOfTurn && released)) {
                turn++;
                EndOfTurn = 0;
                if (turn >= players.size()) {
                    turn = 1;
                }
                while (players[turn].ownership().size() == 0) {
                    turn++;
                    if (turn >= players.size()) {
                        turn = 1;
                    }
                }
            }
        }

        if (players[turn].ownership().size() == 0) {
            turn++;
            if (turn >= players.size()) {
                turn = 1;
            }
        }

        released = 0;
        clickedAt = Tile(0);
        indicator.setFillColor(players[turn].playersColor());

        duplicatesCheck(players);

        //rysowanko

        for (auto player : players) {
            for (auto val : player.ownership()) {
                window.draw(val);
                if (val.value() > 0) {
                    text.setString(std::to_string(val.value()));
                    text.setPosition(val.getPosition());
                    text.move(13, 11);
                    window.draw(text);
                }
            }
        }
        window.draw(indicator);
        window.display();
    }

    return 0;
}
