#ifndef BANNER_H
#define BANNER_H
#include "GameItself/player.h"

class Banner : public sf::RectangleShape
{

public:
    Banner(sf::Vector2f pos, sf::Vector2f size, sf::Font &font);
    Banner();

    void refreshBanner(unsigned long long &pointsLeft, Player &turnOwner, bool &pointsGiveAway);
    void drawMe(sf::RenderTarget &window);

private:
    sf::Text p_text;
};

#endif // BANNER_H
