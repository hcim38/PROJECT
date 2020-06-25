#include "game.h"

Game::Game()
{
    qrTexturePtr = new QResource(":/Textures/Resources/hex-tex.png");
    qrFontPtr = new QResource(":/Fonts/Resources/Lato-Regular.ttf");

    texture.loadFromMemory(qrTexturePtr->data(), qrTexturePtr->size()); //lading resources
    font.loadFromMemory(qrFontPtr->data(), qrFontPtr->size());

    delete qrTexturePtr;
    delete qrFontPtr;

    generateTemplate();
    clickedAt = Tile();
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

void Game::generateTemplate(sf::Vector2i tileSize, unsigned int mapSize)
{
    std::vector<Tile> m_objects;

    for (int i = 0; i < mapSize; ++i)
        for ( int j = 0; j < mapSize; ++j) {
            if (j % 2 == 0) {
                Tile temp(texture, tileSize, sf::Vector2f(i * tileSize.x * 2, 2 * j * tileSize.y));
                temp.setUpTile(tileSize, sf::Vector2i(i, j), 0);
                m_objects.emplace_back(temp);
            } else {
                Tile temp(texture,
                          tileSize,
                          sf::Vector2f(i * tileSize.x * 2 + tileSize.x, 2 * j * tileSize.y));
                temp.setUpTile(tileSize, sf::Vector2i(i, j), 1);
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

void Game::gameLoop()
{
    Lines = createLines(players);
    TilesOnScreen = MAP.size();

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
            players[turn].addPointsToTiles(clickedAt, pointsLeft);
        }

        players[turn].hilightOrigin();

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

        for (auto &player : players) {
            if (player.ownership().empty() && player.nickname() != "MAP") {
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

    for (auto &player : players) {
        if (!player.ownership().empty() && player.nickname() != "MAP" && winCondition >= 2) {
            QMessageBox msg;
            msg.setText(QString::fromStdString(player.nickname()) + " has won the game");
            msg.exec();
        }
    }
    return;
}

void Game::loadMap(QString path, std::vector<Player> &NewPlayers)
{
    players = NewPlayers;
    QFile file(path);
    file.open(QFile::ReadOnly);
    if (file.isOpen()) {
        std::vector<sf::Vector2i> deleted;
        QDataStream in(&file);
        QString str;
        int x, y;

        while (!in.atEnd()) { //TODO get rid of str and translate current maps
            in >> x >> str >> y >> str;
            deleted.emplace_back(sf::Vector2i(x, y));
        }
        file.close();
        for (auto const &pos : deleted) {
            for (auto tile = MAP.begin(); tile != MAP.end(); tile++) {
                if (*tile == pos) {
                    tile->setColor(sf::Color(255, 0, 0, 100));
                    MAP.erase(tile);
                    tile--;
                    continue;
                }
            }
        }
    }

    captureRandomTiles(MAP, players);
}

Game::~Game() {}
