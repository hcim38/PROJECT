#ifndef BANNER_H
#define BANNER_H
#include "GameItself\player.h"

class Banner : public sf::RectangleShape
{
public:
    sf::Text p_text;

public:
    Banner(sf::Vector2f pos, sf::Vector2f size, sf::Font &font);
    Banner();

    void refreshBanner(unsigned long long &pointsLeft, Player &turnOwner, bool &pointsGiveAway)
    {
        setFillColor(turnOwner.playersColor());

        if (pointsGiveAway) {
            p_text.setString("Points Left for player " + turnOwner.nickname() + ":   "
                             + std::to_string(pointsLeft));
        } else if (!turnOwner.AI()) {
            p_text.setString("Now playing " + turnOwner.nickname());
        } else if (turnOwner.AI()) {
            p_text.setString("Computer is making its moves " + turnOwner.nickname());
        }

        p_text.setPosition(((getPosition().x + (getLocalBounds().width / 2))
                            - (p_text.getLocalBounds().width / 2)),
                           ((getPosition().y + (getLocalBounds().height / 2))
                            - p_text.getLocalBounds().height / 2)
                               - 5);
    }

    void drawMe(sf::RenderTarget &window)
    {
        window.draw(*this);
        window.draw(p_text);
    }
};

#endif // BANNER_H
