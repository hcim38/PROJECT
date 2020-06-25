#include "player.h"

Player::Player(QString nickname, int posInVec, bool AI)
{
    p_AI = AI;
    p_nickname = nickname;
    switch (posInVec) {
    case 0:
        p_playersColor = sf::Color(255, 255, 255, 255);
        break;
    case 1:
        p_playersColor = sf::Color::Cyan;
        p_playersColor.a = 150;
        break;
    case 2:
        p_playersColor = sf::Color::Green;
        p_playersColor.a = 150;
        break;
    case 3:
        p_playersColor = sf::Color::Red;
        p_playersColor.a = 150;
        break;
    case 4:
        p_playersColor = sf::Color::Magenta;
        p_playersColor.a = 150;
        break;
    case 5:
        p_playersColor = sf::Color::Yellow;
        p_playersColor.a = 150;
        break;
    default:
        p_playersColor = sf::Color::Black;
        p_playersColor.a = 150;

        break;
    }
}

Player::Player(std::vector<Tile> map)
{
    p_nickname = "MAP";
    p_playersColor = sf::Color(255, 255, 255, 255);
    for (auto val : map) {
        p_ownership.emplace_back(val);
    }
}

std::vector<Tile> Player::ownership()
{
    return p_ownership;
}

sf::Color Player::playersColor()
{
    return p_playersColor;
}

sf::Color Player::playersColorH()
{
    sf::Color col = p_playersColor;
    col.a = 255;
    return col;
}

bool Player::AI()
{
    return p_AI;
}

bool Player::addPointsToTiles(Tile &clickedAt, unsigned long long &pointsLeft)
{
    if (pointsLeft > 0) {
        for (auto &tile : p_ownership) {
            if (tile == clickedAt && tile.getColor() == playersColor()) {
                tile.valPlus1(pointsLeft);
            }
        }
        if (pointsLeft == 0) {
            return true;
        }
    }
    return false;
}

void Player::hilightOrigin()
{
    sf::Color actual;

    for (auto &tile : p_ownership) {
        if (tile.origin()) {
            actual = tile.getColor();
            actual.a = 255;
            tile.setColor(actual);
        }
    }
}

std::string Player::nickname()
{
    return p_nickname.toStdString();
}

void Player::capture(Tile target, Player &loser)
{
    addTileOwnership(target);
    loser.removeOwnership(target);
}

void Player::addTileOwnership(Tile target)
{
    p_ownership.emplace_back(target);
}

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
