#include "player.h"

// Constructors
Player::Player(size_t health, size_t player_strength,
               size_t mana, QPair<size_t, size_t> pos)
    :Object(OBJ_PLAYER, health, mana, pos), strength(player_strength),
      direction(UP){}

Player::Player(QPair<size_t, size_t> pos)
    :Object(OBJ_PLAYER, 100, 100, pos),
      strength(30), direction(UP){}

// Strength
void Player::set_strength(size_t player_strength)
{
    strength = player_strength;
}

size_t Player::get_strength()
{
    return strength;
}

// Direction
void Player::set_direction(edirection dir)
{
    direction = dir;
}

edirection Player::get_direction()
{
    return direction;
}

std::ostream& operator<<(std::ostream& stream, const Player& player)
{
    std::string direction[] = {"UP", "DOWN", "LEFT", "RIGHT"};
    QPair<size_t, size_t> pos = player.position;

    stream << "Player:\n  Position: " << std::to_string(pos.second) << ':' << std::to_string(pos.first)
       << "\n  Direction: " << direction[player.direction]
       << "\n  HP: " << std::to_string(player.hp)
       << "\n  MP: " << std::to_string(player.mp)
       << "\n  Strength: " << std::to_string(player.strength) << std::endl;
    return stream;
}


void Player::write(QJsonObject &json) const
{

    json["strength"] = (int)strength;
    json["direction"] = direction;
    json["x"] = (int)position.second;
    json["y"] = (int)position.first;
    json["hp"] = hp;
    json["mp"] = (int)mp;
    json["type"] = type;

}
