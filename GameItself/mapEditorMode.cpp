#include "mapEditorMode.h"

std::vector<sf::VertexArray> createLines(std::vector<Tile> &tiles)
{
    sf::VertexArray temp(sf::Lines, 2);
    std::vector<sf::VertexArray> vec;

    for (auto &tileOne : tiles) {
        for (auto &tileTwo : tiles) {
            if (tileOne.movePossible(tileTwo)) {
                temp[0].color = sf::Color(100, 100, 100, 50);
                temp[0].position = sf::Vector2f(tileOne.getGlobalBounds().left
                                                    + (tileOne.getGlobalBounds().width / 2),
                                                tileOne.getGlobalBounds().top
                                                    + (tileOne.getGlobalBounds().height / 2));
                temp[1].color = sf::Color(100, 100, 100, 50);
                temp[1].position = sf::Vector2f(tileTwo.getGlobalBounds().left
                                                    + (tileTwo.getGlobalBounds().width / 2),
                                                tileTwo.getGlobalBounds().top
                                                    + (tileTwo.getGlobalBounds().height / 2));
                vec.emplace_back(temp);
            }
        }
    }
    return vec;
}

bool clicked(sf::Vector2i pos, std::vector<Tile> &tiles, Tile &clickedAt)
{
    for (auto &val : tiles) {
        if (val.getGlobalBounds().contains(pos.x, pos.y)) {
            clickedAt = val;
            return 1;
        }
    }

    return 0;
}

void saveToFile(std::vector<Tile> &map) {}

void mapeditor(Game &game)
{ //creating necessary variables
    std::vector<sf::VertexArray> Lines;
    sf::Color defaultColor, deletedColor(255, 0, 0, 100);
    defaultColor = game.MAP[0].getColor();
    defaultColor.a = 255;

    Lines = createLines(game.MAP); //Lines indicating possible moves
    sf::RenderWindow window(sf::VideoMode(640, 640), "Tile Conqueror - Map edit mode");
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(1);

    mapEditTools tools;
    tools.game = &game;
    tools.show();
    QPoint pos = tools.pos();
    pos.setX(pos.x() + 400);
    tools.move(pos);

    while (window.isOpen() && !game.killTheMapEdit) { //config complete, window created, game starts
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == event.MouseButtonReleased
                && event.mouseButton.button == sf::Mouse::Left) {
                clicked(sf::Mouse::getPosition(window), game.MAP, game.clickedAt);
                clicked(sf::Mouse::getPosition(window), game.deletedTilesMapEdit, game.clickedAt);
            }
            if (event.type == event.KeyReleased && event.key.code == sf::Keyboard::Space) {
                return;
            }
        }

        window.clear(sf::Color::Black);

        for (auto &line : Lines) {
            window.draw(line);
        }

        for (auto it = game.MAP.begin(); it != game.MAP.end(); it++) {
            if (*it == game.clickedAt) {
                it->setColor(deletedColor);
                game.deletedTilesMapEdit.emplace_back(*it);
                game.MAP.erase(it);
                it--;
                game.clickedAt = Tile();
                break;
            }
        }

        for (auto it = game.deletedTilesMapEdit.begin(); it != game.deletedTilesMapEdit.end();
             it++) {
            if (*it == game.clickedAt) {
                it->setColor(defaultColor);
                game.MAP.emplace_back(*it);
                game.deletedTilesMapEdit.erase(it);
                it--;
                break;
            }
        }

        game.clickedAt = Tile();

        if (!game.MAP.empty())
            for (auto &val : game.MAP) {
                window.draw(val);
            }
        if (!game.deletedTilesMapEdit.empty())
            for (auto &val : game.deletedTilesMapEdit) {
                window.draw(val);
            }

        if (game.newTemplateMapEdit) {
            game.deletedTilesMapEdit.clear();
            game.MAP = game.generateTemplate(game.texture);
            sf::Color temp;
            for (auto &val : game.MAP) {
                temp = val.getColor();
                temp.a = 255;
                val.setColor(temp);
            }
            game.newTemplateMapEdit = 0;
        }

        window.display(); //koniec
    }
}
