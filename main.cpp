#include "mainFunctions.h"
#include <QResource>

//BUG mozliwe jest przerwanie czyjejs tury klawiszem 'SPACE'

void duplicatesCheck(std::vector<Player> &Players) //chyba nie znaleziono jeszcze :V
{
    for (auto &PlayerOne : Players) {
        for (auto &tileOne : PlayerOne.ownership()) {
            for (auto &PlayerTwo : Players) {
                if (PlayerOne.playersColor() != PlayerTwo.playersColor()) {
                    for (auto &tileTwo : PlayerTwo.ownership()) {
                        if (tileOne == tileTwo) {
                            std::cout << "DUPLIKAT KLOCA!!" << std::endl;
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

    Tile clickedAt(0);
    sf::Texture m_textures;
    QResource Texture(":/Textures/tiles.png");
    m_textures.loadFromMemory(Texture.data(), Texture.size());

    std::vector<Tile> MAP;
    MAP = loadMap(m_textures, sf::Vector2i(32, 32), 10);

    std::vector<Player> players = setupPlayers(MAP);

    sf::RectangleShape indicator(sf::Vector2f(16, 16));

    unsigned long long turn = 1, pointsLeft = 0;
    bool EndOfTurn = 0, released = 0;
    sf::Text text;
    sf::Font font;
    QResource Font(":/Fonts/Lato-Regular.ttf");
    font.loadFromMemory(Font.data(), Font.size());
    text.setFont(font);
    text.setCharacterSize(8);
    std::cout << std::endl << "Now playing:" << players[turn].nickname() << std::endl << std::endl;
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
                for (auto &player : players) {
                    for (auto &tile : player.m_ownership) { //TODO metoda void remOriginParam()
                        tile.setorigin(false);
                    }
                }
                pointsLeft = players[turn].ownership().size();
                if (EndOfTurn) {
                    released = 1;
                }
                EndOfTurn = 1;
                std::cout << "Points Left for player:" << players[turn].nickname() << " "
                          << pointsLeft << std::endl;
            }
        }

        //NOTE magia wyswietlania
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
                std::cout << std::endl
                          << "Now playing:" << players[turn].nickname() << std::endl
                          << std::endl;
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
