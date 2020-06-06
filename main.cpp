#include "mainFunctions.h"
#include "windows.h"

//TODO add maps
//TODO add save game progress
//TODO add GUI
//TODO add pause menu GUI
//TODO make use of player.nickname
//TODO highscores?
//TODO add randomization factor to fights?

int main()
{
    std::cout << "Welcome to Tile Conqueror" << std::endl
              << "Game is loading the resources now." << std::endl;

    QResource qrTexture(":/Textures/hex-tex.png"); //setting resources
    QResource qrFont(":/Fonts/Lato-Regular.ttf");

    sf::Texture texture;
    sf::Font font;

    texture.loadFromMemory(qrTexture.data(), qrTexture.size()); //lading resources
    font.loadFromMemory(qrFont.data(), qrFont.size());

    sf::Text text; //preparing value of tile diplayed on it
    text.setFont(font);
    text.setCharacterSize(12);

    sf::Text pointsLeftTxT; //preparing text for banner bellow game
    pointsLeftTxT.setFont(font);
    pointsLeftTxT.setCharacterSize(22);
    pointsLeftTxT.setStyle(sf::Text::Bold);

    sf::RectangleShape indicator(sf::Vector2f(640, 32)); //preparing banner itself
    indicator.setPosition(sf::Vector2f(0, 640 - 32));

    std::vector<Tile> MAP; //creating necessary variables
    std::vector<Player> players;
    std::vector<sf::VertexArray> Lines;

    unsigned long long turn = 1, pointsLeft = 0;
    bool EndOfTurn = 0, released = 0;
    int winCondition = 0, TilesOnScreen = 0;
    char choice[1];

    sf::Clock clock;
    Tile clickedAt(0);

    while (1) { //game loaded

        MAP = generateTemplate(texture, sf::Vector2i(30, 30), 10);

        TilesOnScreen = MAP.size(); //win condition

        std::cout << std::endl
                  << "Before we start we need to setup the game" << std::endl
                  << std::endl
                  << "Press '1' if you would like to use default settings" << std::endl
                  << "OR" << std::endl
                  << "Press '2' if you would like to set up the game manualy" << std::endl;

        std::cin >> choice;

        while (1) {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Wait, that's illegal!" << std::endl << "Bots: ";
                std::cin >> choice;
            }

            if (choice[0] == '1') {
                system("cls");
                players = setupPlayers(MAP);                
                break;

            } else if (choice[0] == '2') {
                system("cls");
                manualConfig(MAP, players);
                system("cls");
                break;

            } else {
                std::cout << "Wait, that's illegal!" << std::endl;
            }
        }

        Lines = createLines(players);            //Lines indicating possible moves
        ShowWindow(GetConsoleWindow(), SW_HIDE); //Hide console after config
        sf::RenderWindow window(sf::VideoMode(640, 640), "Tile Conqueror");
        window.setFramerateLimit(60);
        window.setVerticalSyncEnabled(1);

        while (window.isOpen()) { //config complete, window created, game starts
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                if (event.type == event.MouseButtonReleased
                    && event.mouseButton.button == sf::Mouse::Left) {
                    clicked(sf::Mouse::getPosition(window), players, clickedAt);
                }
                if (event.type == event.KeyReleased && event.key.code == sf::Keyboard::Space
                    && !players[turn].AI()) {
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
                }
            }

            //TODO poprawic kolejnosc wywolywania funkcji
            //ogolnie uprzatnac
            //uczytelnic zarzadzanie tura

            window.clear(sf::Color::Black);

            for (auto &line : Lines) { //drawing lines bellow everthing
                window.draw(line);
            }

            for (auto &player : players) { //korekta po nieuzywanych podswietleniach
                player.textCorrection(); //TODO przyciemniac kloce z ktorych oraz na ktore ruch nie jest mozliwy
                player.colorCorrection();
            }

            if (!EndOfTurn) { ///SYF
                Turnmanager(players, clickedAt, turn);
            } else {
                pointsLeftTxT.setString(std::to_string(pointsLeft));

                if (addPointsToTiles(clickedAt, players[turn], pointsLeft)
                    || (EndOfTurn && released)) {
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
            } ///End Of SYF

            released = 0; ///???
            clickedAt = Tile(0);
            indicator.setFillColor(players[turn].playersColor());

            //tutaj podswietlam
            //moze funkcja lub uczytelnienie po prostu
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

            //rysowanko SYF again

            for (auto player : players) {
                for (auto val : player.ownership()) {
                    window.draw(val);
                    if (val.value() > 0) {
                        text.setString(std::to_string(val.value()));
                        text.setPosition(val.getPosition());
                        text.move(11, 7);
                        if (val.value() > 9) {
                            text.move(-3, 0);
                        }
                        window.draw(text);
                    }
                }
            }

            window.draw(indicator); //TODO funkcja banner menager

            if (EndOfTurn) {
                pointsLeftTxT.setString("Points Left for player " + players[turn].nickname()
                                        + ":   " + std::to_string(pointsLeft));
            } else if (!players[turn].AI()) {
                pointsLeftTxT.setString("Now playing " + players[turn].nickname());
            } else if (players[turn].AI()) {
                pointsLeftTxT.setString("Computer is making its moves" + players[turn].nickname());
            }
            pointsLeftTxT.setPosition(((indicator.getGlobalBounds().left
                                        + (indicator.getLocalBounds().width / 2))
                                       - (pointsLeftTxT.getLocalBounds().width / 2)),
                                      ((indicator.getGlobalBounds().top
                                        + (indicator.getLocalBounds().height / 2))
                                       - pointsLeftTxT.getLocalBounds().height / 2)
                                          - 5);
            window.draw(pointsLeftTxT);
            window.display(); //koniec

            if (TilesOnScreen == players[turn].ownership().size()) //win condition
                winCondition++;
            if (winCondition >= 2)
                break;
        } ///Game ended

        window.close();
        ShowWindow(GetConsoleWindow(), SW_SHOW);

        if (TilesOnScreen == players[turn].ownership().size()) {
            std::cout << "Looks like player " << players[turn].nickname() << " has won the game!"
                      << std::endl;
        } else {
            std::cout << "No one has won the game?" << std::endl;
        }
        std::cout << std::endl << "Press ENTER to play again or 0 to exit" << std::endl;

        while (1) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                MAP.clear();
                players.clear();
                winCondition = 0;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0))
                return 0;
        }
    }
}
