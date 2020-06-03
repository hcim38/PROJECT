#include "player.h"

void Player::removeOwnership(Tile target)
{
    for (auto it = m_ownership.begin(); it != m_ownership.end(); it++)
        if (*it == target) {
            m_ownership.erase(it);
            break;
        }
}

void Player::clearOrigin()
{
    for (auto &val : m_ownership) {
        val.remOrigin();
    }
}

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
