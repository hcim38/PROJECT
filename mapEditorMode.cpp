#include "mapEditorMode.h"

//std::vector<sf::VertexArray> createLines(std::vector<Tile> &tiles)
//{
//    sf::VertexArray temp(sf::Lines, 2);
//    std::vector<sf::VertexArray> vec;

//    for (auto &tileOne : tiles) {
//        for (auto &tileTwo : tiles) {
//            if (tileOne.movePossible(tileTwo)) {
//                temp[0].color = sf::Color(100, 100, 100, 50);
//                temp[0].position = sf::Vector2f(tileOne.getGlobalBounds().left
//                                                    + (tileOne.getGlobalBounds().width / 2),
//                                                tileOne.getGlobalBounds().top
//                                                    + (tileOne.getGlobalBounds().height / 2));
//                temp[1].color = sf::Color(100, 100, 100, 50);
//                temp[1].position = sf::Vector2f(tileTwo.getGlobalBounds().left
//                                                    + (tileTwo.getGlobalBounds().width / 2),
//                                                tileTwo.getGlobalBounds().top
//                                                    + (tileTwo.getGlobalBounds().height / 2));
//                vec.emplace_back(temp);
//            }
//        }
//    }
//    return vec;
//}

//void clicked(sf::Vector2i pos, std::vector<Tile> &tiles, Tile &clickedAt)
//{
//    for (auto &val : tiles) {
//        if (val.getGlobalBounds().contains(pos.x, pos.y)) {
//            clickedAt = val;
//            break;
//        }
//    }
//}

//std::vector<Tile> mapeditor(sf::Texture &texture, std::vector<Tile> &MAP)
//{
//    std::vector<Tile> deleted; //creating necessary variables
//    std::vector<sf::VertexArray> Lines;

//    Tile clickedAt();

//    Lines = createLines(MAP); //Lines indicating possible moves
//    sf::RenderWindow window(sf::VideoMode(640, 640), "Tile Conqueror - Map edit mode");
//    window.setFramerateLimit(60);
//    window.setVerticalSyncEnabled(1);

//    while (window.isOpen()) { //config complete, window created, game starts
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed) {
//                window.close();
//            }
//            if (event.type == event.MouseButtonReleased
//                && event.mouseButton.button == sf::Mouse::Left) {
//                //clicked(sf::Mouse::getPosition(window), MAP, clickedAt);
//            }
//            if (event.type == event.KeyReleased && event.key.code == sf::Keyboard::Space) {
//                return MAP;
//            }
//        }

//        window.clear(sf::Color::Black);

//        for (auto &line : Lines) {
//            window.draw(line);
//        }

//        for (auto it = MAP.begin(); it != MAP.end(); it++) {
//            if (*it == clickedAt) {
//                deleted.emplace_back(*it);
//                MAP.erase(it);
//                it--;
//                Lines = createLines(MAP);
//            }
//        }

//        clickedAt = Tile();

//        for (auto &val : MAP) {
//            window.draw(val);
//        }

//        window.display(); //koniec
//    }
//}
