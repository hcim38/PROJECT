#include "mainFunctions.h"

void clicked(sf::Vector2i pos, std::vector<Player> &players, Tile &clickedAt)
{
    for (auto &player : players) {
        for (auto &val : player.m_ownership) {
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
    return one.m_value < two.m_value;
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
            if (tileTurn.m_value > 1) {
                for (auto &ennemy : players) {
                    if (ennemy.m_playersColor != players[turn].m_playersColor) {
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
                        if (ennemy.m_playersColor == possibleTargets[0].getColor()) {
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
        if (tile.m_value != 12)
            tile.m_value += 1;
    }
}

void Turnmanager(std::vector<Player> &players, Tile &clickedAt, unsigned long long &turn)
{
    if (players[turn].m_AI) {
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
                if (clickedAt == tile1 && !tile1.origin) { // found clicked tile
                    // it's not the origin
                    //
                    for (auto &player2 : players) {               // looking for origin
                        for (auto &tile2 : player2.m_ownership) { //
                            //
                            if (tile2.origin) // found the origin
                            {
                                if (tile2.movePossible(tile1)) {
                                    if (player2.m_playersColor != players[turn].m_playersColor) {
                                        break; //zly wlasciciel
                                    } else {
                                        //wlasciwy owner
                                        //FIGHT tile2 -> tile1
                                        if (tile2.fight(tile1)) {
                                            tile2.origin = 0;
                                            tile1.origin = 1;
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
                            tile3.origin = true; //make new origin
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
            if (tileM.origin) {
                for (auto &player : players) {
                    for (auto &tile : player.m_ownership) {
                        //
                        if (tileM.movePossible(tile) && tileM.m_value > 1
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
        if (tile == clickedAt && tile.getColor() == player.m_playersColor && tile.m_value < 12) {
            tile.m_value += 1;
            pointsLeft -= 1;
            std::cout << "Points Left for player:" << player.m_nickname << " " << pointsLeft
                      << std::endl;
        }
    }
    if (pointsLeft == 0) {
        return true;
    }
    return false;
}
