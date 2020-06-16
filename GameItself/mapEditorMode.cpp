#include "mapEditorMode.h"

MapEditor::MapEditor()
{
    qrTexturePtr = new QResource(":/Textures/Resources/hex-tex.png");
    qrFontPtr = new QResource(":/Fonts/Resources/Lato-Regular.ttf");

    texture.loadFromMemory(qrTexturePtr->data(), qrTexturePtr->size()); //lading resources
    font.loadFromMemory(qrFontPtr->data(), qrFontPtr->size());

    delete qrTexturePtr;
    delete qrFontPtr;

    clickedAt = Tile();

    generateTemplate();
}

std::vector<sf::VertexArray> MapEditor::createLines(std::vector<Tile> &tiles)
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

bool MapEditor::clicked(sf::Vector2i pos, std::vector<Tile> &tiles, Tile &clickedAt)
{
    for (auto &val : tiles) {
        if (val.getGlobalBounds().contains(pos.x, pos.y)) {
            clickedAt = val;
            return 1;
        }
    }

    return 0;
}

void MapEditor::mapeditor()
{
    generateTemplate();
    sf::Color temp;
    for (auto &val : MAP) {
        temp = val.getColor();
        temp.a = 255;
        val.setColor(temp);
    }

    std::vector<sf::VertexArray> Lines;
    sf::Color defaultColor, deletedColor(255, 0, 0, 100);
    defaultColor = MAP[0].getColor();
    defaultColor.a = 255;

    Lines = createLines(MAP); //Lines indicating possible moves
    sf::RenderWindow window(sf::VideoMode(640, 640), "Tile Conqueror - Map edit mode");
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(1);

    while (window.isOpen() && !killTheMapEdit) { //config complete, window created, game starts
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == event.MouseButtonReleased
                && event.mouseButton.button == sf::Mouse::Left) {
                clicked(sf::Mouse::getPosition(window), MAP, clickedAt);
                clicked(sf::Mouse::getPosition(window), deletedTilesMapEdit, clickedAt);
            }
            if (event.type == event.KeyReleased && event.key.code == sf::Keyboard::Space) {
                return;
            }
        }

        window.clear(sf::Color::Black);

        for (auto &line : Lines) {
            window.draw(line);
        }

        for (auto it = MAP.begin(); it != MAP.end(); it++) {
            if (*it == clickedAt) {
                it->setColor(deletedColor);
                deletedTilesMapEdit.emplace_back(*it);
                MAP.erase(it);
                it--;
                clickedAt = Tile();
                break;
            }
        }

        for (auto it = deletedTilesMapEdit.begin(); it != deletedTilesMapEdit.end(); it++) {
            if (*it == clickedAt) {
                it->setColor(defaultColor);
                MAP.emplace_back(*it);
                deletedTilesMapEdit.erase(it);
                it--;
                break;
            }
        }

        clickedAt = Tile();

        if (!MAP.empty())
            for (auto &val : MAP) {
                window.draw(val);
            }
        if (!deletedTilesMapEdit.empty())
            for (auto &val : deletedTilesMapEdit) {
                window.draw(val);
            }

        if (newTemplateMapEdit) {
            deletedTilesMapEdit.clear();
            generateTemplate();
            sf::Color temp;
            for (auto &val : MAP) {
                temp = val.getColor();
                temp.a = 255;
                val.setColor(temp);
            }
            newTemplateMapEdit = 0;
        }

        window.display(); //koniec
    }
}
