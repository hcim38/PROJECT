#include "mainFunctions.h"
//TODO naprawc AI
//WARNING nie do końca widoczna zasadność istnienia klasy TileMap

int main()
{
    //Ustawianie okna gry
    sf::RenderWindow window(sf::VideoMode(640, 640), "Tilemap");
    window.setFramerateLimit(240);
    window.setVerticalSyncEnabled(1);

    //ustawianie podstawowych obiektow na ktorych dziala gra
    TileMap map;
    Tile clickedAt(0);
    if (!map.load("tiles.png", sf::Vector2i(32, 32), 10))
        return -1;
    std::vector<Player> players;
    players.emplace_back(Player(map));
    players.emplace_back(Player("Player01", 1));
    players.emplace_back(Player("Player02", 2, 1));
    players.emplace_back(Player("Player03", 3, 1));
    capture(map.m_objects[35], players[0], players[1]);
    capture(map.m_objects[33], players[0], players[2]);
    capture(map.m_objects[53], players[0], players[3]);
    players[1].m_ownership[0].m_value = 12;
    players[2].m_ownership[0].m_value = 7;
    players[3].m_ownership[0].m_value = 3;

    sf::RectangleShape indicator(sf::Vector2f(16, 16));

    unsigned long long turn = 1, pointsLeft = 0;
    bool EndOfTurn = 0, released = 0;
    sf::Text text;
    sf::Font font;
    font.loadFromFile("Lato-Regular.ttf");
    text.setFont(font);
    text.setCharacterSize(8);
    std::cout << std::endl << "Now playing:" << players[turn].m_nickname << std::endl << std::endl;
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
                    for (auto &tile : player.m_ownership) {
                        tile.setorigin(false);
                    }
                }
                pointsLeft = players[turn].m_ownership.size();
                if (EndOfTurn) {
                    released = 1;
                }
                EndOfTurn = 1;            
                std::cout << "Points Left for player:" << players[turn].m_nickname << " "
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
                while (players[turn].m_ownership.size() == 0) {
                    turn++;
                    if (turn >= players.size()) {
                        turn = 1;
                    }
                }
                std::cout << std::endl
                          << "Now playing:" << players[turn].m_nickname << std::endl
                          << std::endl;
            }
        }

        if (players[turn].m_ownership.size() == 0) {
            turn++;
            if (turn >= players.size()) {
                turn = 1;
            }
        }

        released = 0;
        clickedAt = Tile(0);
        indicator.setFillColor(players[turn].m_playersColor);
        //rysowanko
        for (auto player : players) {
            for (auto val : player.m_ownership) {
                window.draw(val);
                text.setString(std::to_string(val.m_value));
                text.setPosition(val.getPosition());
                text.move(13, 11);
                window.draw(text);
            }
        }
        window.draw(indicator);
        window.display();
    }

    return 0;
}
