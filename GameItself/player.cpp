#include "player.h"

void Player::removeOwnership(Tile target)
{
    for (auto it = p_ownership.begin(); it != p_ownership.end(); it++)
        if (*it == target) {
            p_ownership.erase(it);
            break;
        }
}
void Player::clearOrigin()
{
    for (auto &val : p_ownership) {
        val.remOrigin();
    }
}
void Player::colorCorrection()
{
    for (auto &val : p_ownership) {
        val.setColor(p_playersColor);
    }
}
void Player::textCorrection()
{
    for (auto &val : p_ownership) {
        val.setTextureRect(
            sf::IntRect(val.value() * val.tilesize().x, 0, val.tilesize().x, val.tilesize().y));
    }
}
