#include "banner.h"

Banner::Banner(sf::Vector2f pos, sf::Vector2f size, sf::Font &font) : p_font(font)
{
    setSize(size);
    setPosition(pos);
    p_text.setFont(font);
    p_text.setCharacterSize(24);
    p_text.setStyle(sf::Text::Bold);
}

Banner::Banner() {}
