#include "player.h"

void Player::colorCorrection()
{
    for (auto &val : m_ownership) {
        val.setColor(m_playersColor);
    }
}

void Player::textCorrection()
{
    for (auto &val : m_ownership) {
        val.setTextureRect(
            sf::IntRect(val.value() * val.tilesize().x, 0, val.tilesize().x, val.tilesize().y));
    }
}
