#include "functionsOutsideClasses.h"

bool sortByScore(const Tile &one, const Tile &two)
{
    return one.value() < two.value();
}

void AI(std::vector<Player> &players, unsigned long long &turn)
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
                                players[turn].capture(enemyTile, enemy);
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
                                players[turn].capture(enemyTile, enemy);
                            }
                        }
                    }
                }
            }
        }
    }
}

void captureRandomTiles(std::vector<Tile> &MAP, std::vector<Player> &players)
{
    players[0].p_ownership = MAP;

    QRandomGenerator randomizer(QRandomGenerator::securelySeeded());
    QString playerName;

    for (auto it = players.begin() + 1; it != players.end(); it++) {
        if (!players[0].ownership().empty()) {
            it->capture(players[0]
                            .ownership()[randomizer.bounded(0, (players[0].p_ownership.size()))],
                        players[0]);
        }
    }
    for (auto it = players.end() - 1; it != players.begin() + 1; it--) {
        if (it->ownership().empty()) {
            players.erase(it);
        }
    }
    for (auto it = players.begin() + 1; it != players.end(); it++) {
        it->p_ownership[0].setBegginerValue();
    }
}

void plus1ForEveryone(std::vector<Tile> &tiles)
{
    unsigned long long pointsLeft = tiles.size(), Full = 0;
    while (pointsLeft > 0) {
        for (auto &tile : tiles) {
            if (tile.p_value < 12 && pointsLeft > 0) {
                tile.p_value += 1;
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

std::vector<Player> setupPlayers(int playersInGame, int AIplayersInGame, std::vector<Tile> &MAP)
{
    QRandomGenerator randomizer(QRandomGenerator::securelySeeded());
    std::vector<Player> players;
    QString playerName;
    players.emplace_back(Player(MAP));

    int human = playersInGame - AIplayersInGame;

    for (int i = 1; i <= playersInGame; i++) {
        playerName = "Player ";
        playerName + i;

        players.emplace_back(Player(playerName, i, (i > human)));
    }

    for (auto it = players.begin() + 1; it != players.end(); it++) {
        it->capture(players[0]
                        .ownership()[randomizer.bounded(0, (players[0].p_ownership.size() - 1))],
                    players[0]);
    }
    for (auto it = players.begin() + 1; it != players.end(); it++) {
        it->p_ownership[0].setBegginerValue();
    }

    return players;
}
