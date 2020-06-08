#include "gameMain.h"

//TODO add distribution of forces bar to Banner
//TODO add save game progress

//TODO add pause menu GUI
//TODO make use of player.nickname
//TODO highscores?
//TODO add randomization factor to fights?

int game1(Game &game)
{
    game.winCondition = 0;
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
                game.clicked(sf::Mouse::getPosition(window), game.players, game.clickedAt);
            }
            if (event.type == event.KeyReleased && event.key.code == sf::Keyboard::Space
                && !game.players[game.turn].AI()) {
                if (game.pointsGiveAway) {
                    game.nextTurn(game.turn, game.players);
                    game.pointsGiveAway = 0;
                    //if points left add 1 point from the remaining to every tile IF possible TODO long
                    continue;
                }
                game.players[game.turn].clearOrigin();
                game.pointsGiveAway = 1;
                game.pointsLeft = game.players[game.turn].ownership().size();
            }
        }

        window.clear(sf::Color::Black);

        for (auto &line : game.Lines) {
            window.draw(line);
        }

        for (auto &player : game.players) {
            player.textCorrection();
            player.colorCorrection();
        }

        //if (!game.players[game.turn].AI()
        //  || (game.clock.getElapsedTime().asSeconds() - game.time.asSeconds() >= 0.5f)) {
        game.time = game.clock.getElapsedTime();
        if (!game.pointsGiveAway) {
            game.Turnmanager(game.players, game.clickedAt, game.turn);
        } else {
            game.addPointsToTiles(game.clickedAt, game.players[game.turn], game.pointsLeft);
        }
        // }

        game.hilightOrigin(game.players[game.turn]);

        game.clickedAt = Tile();

        ////

        for (auto player : game.players) {
            for (auto val : player.ownership()) {
                val.drawMe(window, game.font);
            }
        }

        game.banner.refreshBanner(game.pointsLeft, game.players[game.turn], game.pointsGiveAway);
        game.banner.drawMe(window);

        window.display(); //koniec

        if (game.TilesOnScreen == game.players[game.turn].ownership().size()) //win condition
            game.winCondition++;
        if (game.winCondition >= 2)
            break;
    } ///Game ended

    window.close();
    return 1;
}
