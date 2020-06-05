#include "mainFunctions.h"
#include "windows.h"

void clicked(sf::Vector2i pos, std::vector<Player> &players, Tile &clickedAt)
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

void capture(Tile target, Player &loser, Player &winner)
{
    winner.addTileOwnership(target);
    loser.removeOwnership(target);
}

bool sortByScore(const Tile &one, const Tile &two)
{
    return one.value() < two.value();
}

void AI(std::vector<Player> &players, unsigned long long &turn)
{
    std::vector<Tile> possibleMoves;
    Tile origin(0);
    //TODO Sprawic aby AI nie bilo sie gdy nie ma szans na wygrana
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
        plus1ForEveryone(players[turn].m_ownership);
        turn++;
        if (turn >= players.size()) {
            turn = 1;
        }
        return;
    }

    std::sort(possibleMoves.begin(), possibleMoves.end(), sortByScore);

    for (auto &attacker : players[turn].m_ownership) {
        if (attacker == origin) {
            for (auto &enemy : players) {
                if (enemy.playersColor() == possibleMoves[0].getColor()) {
                    for (auto &enemyTile : enemy.m_ownership) {
                        if (enemyTile == possibleMoves[0]) {
                            //Found the exact tile to fight
                            if (attacker.fight(enemyTile)) {
                                capture(possibleMoves[0], enemy, players[turn]);
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
}

void Turnmanager(std::vector<Player> &players, Tile &clickedAt, unsigned long long &turn)
{ //WARNING m_ownership zbyt czesto wykorzystywany
    if (players[turn].AI()) { //TODO obejrzec, prawdopodobnie przyda sie rework/uczytelnienie
        AI(players, turn);
        return;
    }

    //sprawdzanie gdzie kliknieto

    if (clickedAt != Tile(0)) {
        //
        for (auto &player1 : players) {
            for (auto &tile1 : player1.m_ownership) { //
                //
                if (clickedAt == tile1 && !tile1.origin()) { // found clicked tile
                    // it's not the origin
                    //
                    for (auto &player2 : players) {               // looking for origin
                        for (auto &tile2 : player2.m_ownership) { //
                            //
                            if (tile2.origin()) // found the origin
                            {
                                if (tile2.movePossible(tile1)) {
                                    if (player2.playersColor() != players[turn].playersColor()) {
                                        break; //zly wlasciciel
                                    } else {
                                        //wlasciwy owner
                                        //FIGHT tile2 -> tile1
                                        if (tile2.fight(tile1)) {                                          
                                            capture(tile1, player1, player2);
                                            tile2.swapOrigin(tile1);
                                        }
                                        //FIGTH end
                                    }
                                }
                            }
                        }
                    }
                    //origin not found
                    for (auto &tile3 :
                         players[turn]
                             .m_ownership) { //check if player of the actual turn owns this tile
                        //
                        if (clickedAt == tile3) {
                            tile3.makeOrigin();

                            break;
                        }
                    }

                    break;
                }
            }
        }
    }
}

bool addPointsToTiles(Tile &clickedAt, Player &player, unsigned long long &pointsLeft)
{
    for (auto &tile : player.m_ownership) {
        if (tile == clickedAt && tile.getColor() == player.playersColor()) {
            tile.valPlus1(pointsLeft);
            std::cout << "Points Left: " << pointsLeft << std::endl;
        }
    }
    if (pointsLeft == 0) {
        system("cls");
        return true;
    }
    return false;
}

std::vector<Player> setupPlayers(std::vector<Tile> &map, int playersInGame, int AIplayersInGame)
{
    QRandomGenerator randomizer(QRandomGenerator::securelySeeded());
    std::vector<Player> players;
    std::string playerName;
    players.emplace_back(Player(map));

    int human = playersInGame - AIplayersInGame;

    for (int i = 1; i <= playersInGame; i++) {
        playerName = "Player ";
        playerName.append(std::to_string(i));

        players.emplace_back(Player(playerName, i, (i > human)));
    }

    for (auto it = players.begin() + 1; it != players.end(); it++) {
        capture(players[0].ownership()[randomizer.bounded(0, (players[0].m_ownership.size() - 1))],
                players[0],
                *it);
        duplicatesCheck(players);
    }
    for (auto it = players.begin() + 1; it != players.end(); it++) {
        it->m_ownership[0].setBegginerValue();
    }

    return players;
}

void plus1ForEveryone(std::vector<Tile> &tiles)
{
    int pointsLeft = tiles.size(), Full = 0;
    while (pointsLeft > 0) {
        for (auto &tile : tiles) {
            if (tile.m_value < 12) {
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

std::vector<Tile> generateTemplate(sf::Texture &m_textures,
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

std::vector<sf::VertexArray> createLines(std::vector<Player> &players)
{
    sf::VertexArray temp(sf::Lines, 2);
    std::vector<sf::VertexArray> vec;

    for (auto &playerM : players) {
        for (auto &tileOne : playerM.ownership()) {
            for (auto &player : players) {
                for (auto &tileTwo : player.ownership()) {
                    if (tileOne.movePossibleWithOutColor(tileTwo)) {
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

void duplicatesCheck(std::vector<Player> &Players) //chyba nie znaleziono jeszcze :V
{
    for (auto &PlayerOne : Players) {
        for (auto &tileOne : PlayerOne.ownership()) {
            for (auto &PlayerTwo : Players) {
                if (PlayerOne.playersColor() != PlayerTwo.playersColor()) {
                    for (auto &tileTwo : PlayerTwo.ownership()) {
                        if (tileOne == tileTwo) {
                            qDebug() << "DUPLIKAT KLOCA!!";
                        }
                    }
                }
            }
        }
    }
}

void manualConfig(std::vector<Tile> &map, std::vector<Player> &players)
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
