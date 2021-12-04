#include "enemy.h"

Enemy::Enemy(eenemytype type, QPair<size_t, size_t> pos/*, std::shared_ptr<Field> field_ptr*/)
    :Object(OBJ_ENEMY, 50, 0, pos), enemy_type(type)/*,field(field_ptr)*/
{
    if (enemy_type == KAMIKAZE)
    {
        set_hp(20);
        set_strength(50);
    }
    else if (enemy_type == ARCHER)
    {
        set_hp(50);
        set_strength(10);
    }
    else
    {
        set_strength(30);
    }
}


void Enemy::set_strength(size_t str)
{
    strength = str;
}

size_t Enemy::get_strength()
{
    return strength;
}

eenemytype Enemy::get_enemy_type()
{
    return enemy_type;
}


std::ostream& operator<<(std::ostream& stream, const Enemy& enemy)
{
    std::string type[] = {"KAMIKAZE", "ARCHER", "MELEE"};
    QPair<size_t, size_t> pos = enemy.position;

    stream << "Enemy:\n  Position: " << std::to_string(pos.second) << ':' << std::to_string(pos.first)
       << "\n  Type: " << type[enemy.enemy_type]
       << "\n  HP: " << std::to_string(enemy.hp)
       << "\n  MP: " << std::to_string(enemy.mp)
       << "\n  Strength: " << std::to_string(enemy.strength) << std::endl;
    return stream;
}
