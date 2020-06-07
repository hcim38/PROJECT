#include "mainFunctions.h"

//TODO add distribution of forces bar to Banner
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

    Banner banner(sf::Vector2f(0, 640 - 32), sf::Vector2f(640, 32), font);

    std::vector<Tile> MAP; //creating necessary variables
    std::vector<Player> players;
    std::vector<sf::VertexArray> Lines;

    unsigned long long turn = 1, pointsLeft = 0;
    bool pointsGiveAway = 0;
    unsigned int winCondition = 0, TilesOnScreen = 0;
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

        while (1) {
            std::cin >> choice;
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
                    if (pointsGiveAway) {
                        nextTurn(turn, players);
                        pointsGiveAway = 0;
                        //if points left add 1 point from the remaining to every tile IF possible TODO long
                        continue;
                    }
                    players[turn].clearOrigin();
                    pointsGiveAway = 1;
                    pointsLeft = players[turn].ownership().size();
                }
            }

            window.clear(sf::Color::Black);

            for (auto &line : Lines) {
                window.draw(line);
            }

            for (auto &player : players) {
                player.textCorrection();
                player.colorCorrection();
            }

            if (!pointsGiveAway) {
                Turnmanager(players, clickedAt, turn);
            } else {
                addPointsToTiles(clickedAt, players[turn], pointsLeft);
            }

            hilightOrigin(players[turn]);

            clickedAt = Tile(0);

            ////

            for (auto player : players) {
                for (auto val : player.ownership()) {
                    val.drawMe(window, font);
                }
            }

            banner.refreshBanner(pointsLeft, players[turn], pointsGiveAway);
            banner.drawMe(window);

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
                break;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0))
                return 0;
        }
    }
}
