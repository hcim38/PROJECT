#include "banner.h"

Banner::Banner(sf::Vector2f pos, sf::Vector2f size, sf::Font &font)
{
    setSize(size);
    setPosition(pos);
    p_text.setFont(font);
    p_text.setCharacterSize(24);
    p_text.setStyle(sf::Text::Bold);
}

Banner::Banner() {}

void Banner::refreshBanner(unsigned long long &pointsLeft, Player &turnOwner, bool &pointsGiveAway)
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

void Banner::drawMe(sf::RenderTarget &window)
{
    window.draw(*this);
    window.draw(p_text);
}
