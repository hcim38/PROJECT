#include "game.h"

Game::Game()
{
    qrTexturePtr = new QResource(":/Textures/Resources/hex-tex.png");
    qrFontPtr = new QResource(":/Fonts/Resources/Lato-Regular.ttf");

    texture.loadFromMemory(qrTexturePtr->data(), qrTexturePtr->size()); //lading resources
    font.loadFromMemory(qrFontPtr->data(), qrFontPtr->size());

    delete qrTexturePtr;
    delete qrFontPtr;

    clickedAt = Tile();

    MAP = generateTemplate(texture, sf::Vector2i(30, 30), 10);

    players = setupPlayers(MAP);

    TilesOnScreen = MAP.size(); //win condition

    banner = Banner(sf::Vector2f(0, 640 - 32), sf::Vector2f(640, 32), font);
    Lines = createLines(players); //Lines indicating possible moves
}

void Game::clicked(sf::Vector2i pos, std::vector<Player> &players, Tile &clickedAt)
{
    for (auto &player : players) {
        for (auto &val : player.ownership()) {
            if (val.getGlobalBounds().contains(pos.x, pos.y)) {
                clickedAt = val;
                break;
            }
        }
    }
}

void Game::capture(Tile target, Player &loser, Player &winner)
{
    winner.addTileOwnership(target);
    loser.removeOwnership(target);
}

bool sortByScore(const Tile &one, const Tile &two)
{
    return one.value() < two.value();
}

void Game::AI(std::vector<Player> &players, unsigned long long &turn)
{
    std::vector<Tile> possibleMoves;
    Tile origin;

    for (auto &turnOwnerTile : players[turn].ownership()) {
        if (turnOwnerTile.value() > 1) {
            for (auto &enemy : players) {
                for (auto &enemyTile : enemy.ownership()) {
                    if (turnOwnerTile.movePossible(enemyTile)
                        && turnOwnerTile.getColor() != enemyTile.getColor()) {
                        if ((enemyTile.value() < turnOwnerTile.value() - 1)
                            || turnOwnerTile.value() > 5)
                            possibleMoves.emplace_back(enemyTile);
                    }
                }
            }
            if (possibleMoves.size() > 0) {
                origin = turnOwnerTile;
                break;
            }
        }
    }

    if (possibleMoves.size() == 0) {
        plus1ForEveryone(players[turn].p_ownership);
        turn++;
        if (turn >= players.size()) {
            turn = 1;
        }
        return;
    }

    std::sort(possibleMoves.begin(), possibleMoves.end(), sortByScore);

    for (auto &attacker : players[turn].p_ownership) {
        if (attacker == origin) {
            for (auto &enemy : players) {
                if (enemy.playersColor() == possibleMoves[0].getColor()) {
                    for (auto &enemyTile : enemy.p_ownership) {
                        if (enemyTile == possibleMoves[0]) {
                            //Found the exact tile to fight
                            if (attacker.fight(enemyTile)) {
                                capture(enemyTile, enemy, players[turn]);
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
}

void Game::Turnmanager(std::vector<Player> &players, Tile &clickedAt, unsigned long long &turn)
{
    if (players[turn].AI()) {
        AI(players, turn);
        return;
    }

    if (clickedAt == Tile())
        return;
    if (clickedAt.getColor() == players[turn].playersColorH())
        return;

    if (clickedAt.getColor() == players[turn].playersColor()) {
        players[turn].clearOrigin();
        for (auto &newOrigin : players[turn].p_ownership) {
            if (clickedAt == newOrigin) {
                newOrigin.makeOrigin();
                return;
            }
        }
    }

    if (clickedAt.getColor() != players[turn].playersColor()) {
        //find the origin tile at players vector
        auto turnOwnerTile = players[turn].p_ownership.begin();
        for (; turnOwnerTile != players[turn].p_ownership.end(); turnOwnerTile++) {
            if (turnOwnerTile->origin()) {
                break;
            }
        }

        //now find target at players vector
        for (auto &enemy : players) {
            if (enemy.playersColor() == clickedAt.getColor()) {
                for (auto &enemyTile : enemy.p_ownership) {
                    if (enemyTile == clickedAt) {
                        //FOUND now fight
                        if (turnOwnerTile->movePossible(enemyTile)) {
                            //move possible
                            if (turnOwnerTile->fight(enemyTile)) {
                                turnOwnerTile->swapOrigin(enemyTile);
                                capture(enemyTile, enemy, players[turn]);
                            }
                        }
                    }
                }
            }
        }
    }
}

bool Game::addPointsToTiles(Tile &clickedAt, Player &player, unsigned long long &pointsLeft)
{
    if (pointsLeft > 0) {
        for (auto &tile : player.p_ownership) {
            if (tile == clickedAt && tile.getColor() == player.playersColor()) {
                tile.valPlus1(pointsLeft);
                std::cout << "Points Left: " << pointsLeft << std::endl;
            }
        }
        if (pointsLeft == 0) {
            system("cls");
            return true;
        }
    }
    return false;
}

std::vector<Player> Game::setupPlayers(std::vector<Tile> &map,
                                       int playersInGame,
                                       int AIplayersInGame)
{
    QRandomGenerator randomizer(QRandomGenerator::securelySeeded());
    std::vector<Player> players;
    QString playerName;
    players.emplace_back(Player(map));

    int human = playersInGame - AIplayersInGame;

    for (int i = 1; i <= playersInGame; i++) {
        playerName = "Player ";
        playerName + i;

        players.emplace_back(Player(playerName, i, (i > human)));
    }

    for (auto it = players.begin() + 1; it != players.end(); it++) {
        capture(players[0].ownership()[randomizer.bounded(0, (players[0].p_ownership.size() - 1))],
                players[0],
                *it);
    }
    for (auto it = players.begin() + 1; it != players.end(); it++) {
        it->p_ownership[0].setBegginerValue();
    }

    return players;
}

void Game::plus1ForEveryone(std::vector<Tile> &tiles)
{
    unsigned long long pointsLeft = tiles.size(), Full = 0;
    while (pointsLeft > 0) {
        for (auto &tile : tiles) {
            if (tile.m_value < 12 && pointsLeft > 0) {
                tile.m_value += 1;
                pointsLeft -= 1;
            } else {
                Full++;
            }
        }
        if (Full >= tiles.size()) {
            break;
        }
        Full = 0;
    }
}

std::vector<Tile> Game::generateTemplate(sf::Texture &m_textures,
                                         sf::Vector2i tileSize,
                                         unsigned int mapSize)
{
    std::vector<Tile> m_objects;

    for (unsigned int i = 0; i < mapSize; ++i)
        for (unsigned int j = 0; j < mapSize; ++j) {
            if (j % 2 == 0) {
                Tile temp(m_textures,
                          tileSize,
                          sf::Vector2f(i * tileSize.x * 2, 2 * j * tileSize.y));
                temp.m_tilesize = tileSize;
                temp.m_position = sf::Vector2i(i, j);
                temp.offset = 0;
                m_objects.emplace_back(temp);
            } else {
                Tile temp(m_textures,
                          tileSize,
                          sf::Vector2f(i * tileSize.x * 2 + tileSize.x, 2 * j * tileSize.y));
                temp.m_tilesize = tileSize;
                temp.m_position = sf::Vector2i(i, j);
                temp.offset = 1;
                m_objects.emplace_back(temp);
            }
        }
    for (auto &obj : m_objects) {
        obj.move(tileSize.x / 2, tileSize.y / 2);
    }

    return m_objects;
}

std::vector<sf::VertexArray> Game::createLines(std::vector<Player> &players)
{
    sf::VertexArray temp(sf::Lines, 2);
    std::vector<sf::VertexArray> vec;

    for (auto &playerM : players) {
        for (auto &tileOne : playerM.ownership()) {
            for (auto &player : players) {
                for (auto &tileTwo : player.ownership()) {
                    if (tileOne.movePossible(tileTwo)) {
                        temp[0].color = sf::Color(100, 100, 100, 50);
                        temp[0].position = sf::Vector2f(tileOne.getGlobalBounds().left
                                                            + (tileOne.getGlobalBounds().width / 2),
                                                        tileOne.getGlobalBounds().top
                                                            + (tileOne.getGlobalBounds().height
                                                               / 2));
                        temp[1].color = sf::Color(100, 100, 100, 50);
                        temp[1].position = sf::Vector2f(tileTwo.getGlobalBounds().left
                                                            + (tileTwo.getGlobalBounds().width / 2),
                                                        tileTwo.getGlobalBounds().top
                                                            + (tileTwo.getGlobalBounds().height
                                                               / 2));
                        vec.emplace_back(temp);
                    }
                }
            }
        }
    }
    return vec;
}

void Game::manualConfig(std::vector<Tile> &map, std::vector<Player> &players)
{
    int playersInGame, botsInGame;
    while (1) {
        std::cout
            << "Set up how many players in range from 2 to 5 you would like to have including AI"
            << std::endl
            << "In this game less than 2 players is meaningless and more than 5 players is "
               "unsupported"
            << std::endl
            << "Players: ";

        std::cin >> playersInGame;
        while (1) {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Wait, that's illegal!" << std::endl << "Players: ";
                std::cin >> playersInGame;
            }
            if (!std::cin.fail()) {
                break;
            }
        }
        std::cout << std::endl
                  << "Now how many of those " << playersInGame << " would like to be AI?"
                  << std::endl
                  << "Bots: ";
        std::cin >> botsInGame;
        while (1) {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Wait, that's illegal!" << std::endl << "Bots: ";
                std::cin >> botsInGame;
            } else {
                break;
            }
        }
        std::cout << std::endl;

        if (playersInGame <= 5 && playersInGame > 0 && botsInGame <= playersInGame) {
            players = setupPlayers(map, playersInGame, botsInGame);
            break;
        } else {
            std::cout << "Those values are incorrect!" << std::endl;
            std::cout << "Try again" << std::endl << std::endl;
        }
    }
}

void Game::nextTurn(unsigned long long &turn, std::vector<Player> &players)
{
    turn++;
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

void Game::hilightOrigin(Player &player)
{
    sf::Color actual;

    for (auto &tile : player.p_ownership) {
        if (tile.origin()) {
            actual = tile.getColor();
            actual.a = 255;
            tile.setColor(actual);
        }
    }
}
