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
    //Sprawic aby AI nie bilo sie gdy nie ma szans na wygrana
    for (auto &turnOwnerTile : players[turn].ownership()) {
        if (turnOwnerTile.value() > 1) {
            for (auto &enemy : players) {
                for (auto &enemyTile : enemy.ownership()) {
                    if (turnOwnerTile.movePossible(enemyTile)
                        && turnOwnerTile.getColor() != enemyTile.getColor()) {
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
                                            tile2.swapOrigin(tile1);

                                            capture(tile1, player1, player2);
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
        capture(map[randomizer.bounded(0, (players[0].m_ownership.size() - 1))], players[0], *it);
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

std::vector<Tile> loadMap(sf::Texture &m_textures, sf::Vector2i tileSize, unsigned int mapSize)
{
    std::vector<Tile> m_objects;

    for (unsigned int i = 0; i < mapSize; ++i)
        for (unsigned int j = 0; j < mapSize;
             ++j) { //petla wypelniajaca vector do ustalonych rozmiarow
            Tile temp(m_textures,
                      tileSize,
                      sf::Vector2f(i * tileSize.x * 2 + 16, 2 * j * tileSize.y + 16));
            temp.m_tilesize = tileSize;           //ustalenie rozmiaru obiektu
            temp.m_position = sf::Vector2i(i, j); //ustalenie pozycji
            m_objects.emplace_back(temp);
        }

    return m_objects;
}
