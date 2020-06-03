#include "mainFunctions.h"

int main()
{
    //Ustawianie okna gry
    std::cout << "Welcome to Tile Conqueror game" << std::endl;
    QResource qrTexture(":/Textures/hex-tex.png");
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
    MAP = loadMap(m_textures, sf::Vector2i(30, 30), 10);

    std::cout << "Before we start we need to setup the game" << std::endl
              << "Press '1' if you would like to use default settings" << std::endl
              << "OR" << std::endl
              << "Press '2' if you would like to set up the game manualy" << std::endl;
    char choice = '1';
    //std::cin >> choice;

    std::vector<Player> players;

    if (choice == '1') { //FIXME make it userproof!
        players = setupPlayers(MAP, 5, 5);
        system("cls");
    } else if (choice == '2') {
        system("cls");
        int playersInGame, botsInGame;
        std::cout
            << "Set up how many players in range from 2 to 5 you would like to have including AI"
            << std::endl
            << "In this game less than 2 players is meaningless and more than 5 players is "
               "unsupported"
            << std::endl
            << "Players: ";
        std::cin >> playersInGame;
        std::cout << std::endl
                  << "Now how many of those " << playersInGame << " would like to be AI?"
                  << std::endl
                  << "Bots: ";
        std::cin >> botsInGame;
        std::cout << std::endl;

        players = setupPlayers(MAP, playersInGame, botsInGame);

        system("cls");
    } else {
        std::cout << "Wait, that's illegal!" << std::endl;

        return -1;
    }
    std::vector<sf::VertexArray> Lines = createLines(players);
    sf::RenderWindow window(sf::VideoMode(640, 640), "Tilemap");
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(1);

    float blinkInterval = 0.3f;

    unsigned long long turn = 1, pointsLeft = 0;
    bool EndOfTurn = 0, released = 0, lit = 0;

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == event.MouseButtonReleased
                && event.mouseButton.button == sf::Mouse::Left) {
                clicked(sf::Mouse::getPosition(window), //wywyolanie reakcji na kliknecie
                        players,
                        clickedAt);
            }
            if (event.type == event.KeyReleased && event.key.code == sf::Keyboard::Space) {
                if (!players[turn].AI()) {
                    for (auto &player : players) {
                        if (player.ownership().size() > 0) {
                            player.clearOrigin();
                        }
                    }
                    pointsLeft = players[turn].ownership().size();
                    if (EndOfTurn) {
                        released = 1;
                    }
                    EndOfTurn = 1;
                    std::cout << "Points Left for player: " << players[turn].nickname() << " "
                              << pointsLeft << std::endl;
                }
            }
        }

        //TODO poprawic kolejnosc wywolywania funkcji
        //ogolnie uprzatnac
        //uczytelnic zarzadzanie tura

        window.clear(sf::Color::Black);
        for (auto &line : Lines) {
            window.draw(line);
        }

        for (auto &player : players) { //korekta po nieuzywanych podswietleniach
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

        //tutaj podswietlam
        for (auto &playerM : players) {
            for (auto &tileM : playerM.m_ownership) {
                //
                if (tileM.origin()) {
                    sf::Color actual;
                    for (auto &player : players) {
                        for (auto &tile : player.m_ownership) {
                            //
                            if (tileM.movePossible(tile) && tileM.value() > 1
                                && tileM.getColor() != tile.getColor()) {
                                actual = tile.getColor();
                                actual.a = 150;
                                tile.setColor(actual);
                            }
                        }
                    }
                    break;
                }
            }
        }

        //rysowanko

        for (auto player : players) {
            for (auto val : player.ownership()) {
                window.draw(val);
                if (val.value() > 0) {
                    text.setString(std::to_string(val.value()));
                    text.setPosition(val.getPosition());
                    text.move(13, 10);
                    if (val.value() > 9) {
                        text.move(-3, 0);
                    }
                    window.draw(text);
                }
            }
        }
        if (!EndOfTurn || (clock.getElapsedTime().asSeconds() >= blinkInterval)) {
            //window.draw(indicator);
            if (clock.getElapsedTime().asSeconds() >= 2 * blinkInterval) {
                clock.restart();
            }
        }

        window.display();
    }

    return 0;
}
