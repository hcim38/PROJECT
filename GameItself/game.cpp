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

    generateTemplate();

    players = setupPlayers(MAP);

    TilesOnScreen = MAP.size();

    banner = Banner(sf::Vector2f(0, 640 - 32), sf::Vector2f(640, 32), font);
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
            }
        }
        if (pointsLeft == 0) {
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

void Game::generateTemplate(sf::Vector2i tileSize, unsigned int mapSize)
{
    std::vector<Tile> m_objects;

    for (unsigned int i = 0; i < mapSize; ++i)
        for (unsigned int j = 0; j < mapSize; ++j) {
            if (j % 2 == 0) {
                Tile temp(texture, tileSize, sf::Vector2f(i * tileSize.x * 2, 2 * j * tileSize.y));
                temp.m_tilesize = tileSize;
                temp.m_position = sf::Vector2i(i, j);
                temp.offset = 0;
                m_objects.emplace_back(temp);
            } else {
                Tile temp(texture,
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

    MAP = m_objects;
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

void Game::gameLoop()
{
    Lines = createLines(players);

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
                    //TODO if points left add 1 point from the remaining to every tile IF possible
                    continue;
                }
                players[turn].clearOrigin();
                pointsGiveAway = 1;
                pointsLeft = players[turn].ownership().size();
            } //TODO add full value on PPM
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

        clickedAt = Tile();

        ////

        for (auto player : players) {
            for (auto val : player.ownership()) {
                val.drawMe(window, font);
            }
        }

        banner.refreshBanner(pointsLeft, players[turn], pointsGiveAway);
        banner.drawMe(window);

        window.display(); //koniec

        for (auto const &player : players) {
            if (player.p_ownership.empty() && player.p_nickname != "MAP") {
                playersEmpty++;
            }
        }

        if (TilesOnScreen == players[turn].ownership().size()
            || playersEmpty == players.size() - 2) //win condition
            winCondition++;
        if (winCondition >= 2)
            break;
        playersEmpty = 0;
    } ///Game ended

    window.close();

    for (auto const &player : players) {
        if (!player.p_ownership.empty() && player.p_nickname != "MAP" && winCondition >= 2) {
            QMessageBox msg;
            msg.setText(player.p_nickname + " has won the game");
            msg.exec();
        }
    }
    return;
}

void Game::captureRandomTiles()
{
    players[0].p_ownership = MAP;

    QRandomGenerator randomizer(QRandomGenerator::securelySeeded());
    QString playerName;

    for (auto it = players.begin() + 1; it != players.end(); it++) {
        capture(players[0].ownership()[randomizer.bounded(0, (players[0].p_ownership.size()))],
                players[0],
                *it);
    }
    for (auto it = players.begin() + 1; it != players.end(); it++) {
        it->p_ownership[0].setBegginerValue();
    }
}

void Game::loadMap(QString &path, std::vector<Player> &NewPlayers)
{
    players = NewPlayers;
    QFile file(path);
    file.open(QFile::ReadOnly);
    if (file.isOpen()) {
        generateTemplate();
        std::vector<sf::Vector2i> deleted;
        QDataStream in(&file);
        QString str;
        int x, y;

        while (!in.atEnd()) {
            in >> x >> str >> y >> str;
            deleted.emplace_back(sf::Vector2i(x, y));
        }
        file.close();
        for (auto const &pos : deleted) {
            for (auto tile = MAP.begin(); tile != MAP.end(); tile++) {
                if (pos.x == tile->m_position.x && pos.y == tile->m_position.y) {
                    tile->setColor(sf::Color(255, 0, 0, 100));
                    MAP.erase(tile);
                    tile--;
                    continue;
                }
            }
        }
    }

    captureRandomTiles();
}

Game::~Game() {}
