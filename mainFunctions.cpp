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

    for (auto &turnOwnerTile : players[turn].ownership()) {
        if (turnOwnerTile.value() > 1) {
            for (auto &enemy : players) {
                for (auto &enemyTile : enemy.ownership()) {
                    if (turnOwnerTile.movePossible(enemyTile)
                        && turnOwnerTile.getColor() != enemyTile.getColor()) {
                        //mozliwy ruch
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

    for (auto &attacker : players[turn].m_ownership) { //TODO byc moze integracja capture() z fight()
        if (attacker == origin) {
            if (attacker.fight(possibleMoves[0])) {
                for (auto &enemy : players) {
                    if (enemy.playersColor() == possibleMoves[0].getColor()) {
                        capture(possibleMoves[0], enemy, players[turn]);
                        break;
                    }
                }
            }
        }
    }
}

void Turnmanager(std::vector<Player> &players, Tile &clickedAt, unsigned long long &turn)
{ //WARNING m_ownership zbyt czesto wykorzystywany
    if (players[turn].AI()) {
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
                                            tile2.setorigin(false); //TODO to ma byc priv
                                            tile1.setorigin(true);  //TODO to tez
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
                            tile3.setorigin(true); //make new origin TODO to tez ma byc priv
                            tile3.setColor(sf::Color::Red);
                            break;
                        }
                    }

                    break;
                }
            }
        }
    }

    //po sprawdzeniu gdzie kliknieto szukam origin i podswietlam tilesy do okola
    for (auto &playerM : players) {
        for (auto &tileM : playerM.m_ownership) {
            //
            if (tileM.origin()) {
                for (auto &player : players) {
                    for (auto &tile : player.m_ownership) {
                        //
                        if (tileM.movePossible(tile) && tileM.value() > 1
                            && tileM.getColor() != tile.getColor()) {
                            sf::Color actual = tile.getColor();
                            actual.a = 150;
                            tile.setColor(actual);
                        }
                    }
                }
                break;
            }
        }
    }
}

bool addPointsToTiles(Tile &clickedAt,
                      Player &player,
                      unsigned long long &pointsLeft) //todo przeniesc to do gracza lub tile
{
    for (auto &tile : player.m_ownership) {
        if (tile == clickedAt && tile.getColor() == player.playersColor() && tile.value() < 12) {
            tile.setvalue(tile.value() + 1); //TODO musi byc priv
            pointsLeft -= 1;
            //std::cout << "Points Left for player:" << player.nickname() << " " << pointsLeft
            //<< std::endl;
        }
    }
    if (pointsLeft == 0) {
        return true;
    }
    return false;
}

std::vector<Player> setupPlayers(std::vector<Tile> &map)
{
    std::vector<Player> players;
    players.emplace_back(Player(map));
    players.emplace_back(Player("Player01", 1));
    players.emplace_back(Player("Player02", 2, 1));
    players.emplace_back(Player("Player03", 3, 1));
    //capture(map[35], players[0], players[1]);
    capture(map[33], players[0], players[2]);
    capture(map[53], players[0], players[3]);
    //players[1].m_ownership[0].setBegginerValue();
    players[2].m_ownership[0].setBegginerValue();
    players[3].m_ownership[0].setBegginerValue();

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

void clearOriginParam(std::vector<Tile> &tiles) {}
