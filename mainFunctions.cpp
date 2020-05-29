#include "mainFunctions.h"

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
    std::vector<Tile> possibleTargets;

    bool Changes = 1;
    while (Changes) {
        Changes = 0;

        for (auto &tileTurn : players[turn].m_ownership) {
            //dla kazdego kloca po kolei
            //szuka mozliwych ruchow
            if (tileTurn.value() > 1) {
                for (auto &ennemy : players) {
                    if (ennemy.playersColor() != players[turn].playersColor()) {
                        //jesli jest potencjalnym wrogiem
                        for (auto &ennemyTile : ennemy.m_ownership) {
                            if (tileTurn.movePossible(ennemyTile)) {
                                //ruch z pojedynczego kloca jest mozliwy na kloca wroga
                                possibleTargets.emplace_back(ennemyTile);
                            }
                        }
                    }
                }
                //wybieram ruch
                //
                if (possibleTargets.size() > 0) {
                    std::sort(possibleTargets.begin(), possibleTargets.end(), sortByScore);
                    for (auto &ennemy : players) {
                        if (ennemy.playersColor() == possibleTargets[0].getColor()) {
                            if (tileTurn.fight(possibleTargets[0])) {
                                capture(possibleTargets[0], ennemy, players[turn]);
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    for (auto &tile : players[turn].m_ownership) {
        if (tile.value() != 12)
            tile.setvalue(1 + tile.value());
    }
}

void Turnmanager(std::vector<Player> &players, Tile &clickedAt, unsigned long long &turn)
{
    if (players[turn].AI()) {
        AI(players, turn);
        turn++;
        if (turn >= players.size())
            turn = 1;
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
                                            tile2.setorigin(false);
                                            tile1.setorigin(true);
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
                            tile3.setorigin(true); //make new origin
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

bool addPointsToTiles(Tile &clickedAt, Player &player, unsigned long long &pointsLeft)
{
    for (auto &tile : player.m_ownership) {
        if (tile == clickedAt && tile.getColor() == player.playersColor() && tile.value() < 12) {
            tile.setvalue(tile.value() + 1);
            pointsLeft -= 1;
            std::cout << "Points Left for player:" << player.nickname() << " " << pointsLeft
                      << std::endl;
        }
    }
    if (pointsLeft == 0) {
        return true;
    }
    return false;
}

std::vector<Player> setupPlayers(TileMap &map)
{
    std::vector<Player> players;
    players.emplace_back(Player(map));
    players.emplace_back(Player("Player01", 1));
    players.emplace_back(Player("Player02", 2, 1));
    players.emplace_back(Player("Player03", 3, 1));
    capture(map.m_objects[35], players[0], players[1]);
    capture(map.m_objects[33], players[0], players[2]);
    capture(map.m_objects[53], players[0], players[3]);
    players[1].m_ownership[0].setvalue(2);
    players[2].m_ownership[0].setvalue(2);
    players[3].m_ownership[0].setvalue(2);

    return players;
}
