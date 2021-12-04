#ifndef ENEMY_H
#define ENEMY_H
#include "../Item/item.h"

enum eenemytype {KAMIKAZE, ARCHER, MELEE};

class Enemy : public Object
{
private:
    size_t strength;
    eenemytype enemy_type;
    void set_strength(size_t str);
public:
    Enemy(eenemytype enemy_type, QPair<size_t, size_t> pos);
    size_t get_strength();
    eenemytype get_enemy_type();

    friend std::ostream& operator<<(std::ostream& stream, const Enemy& enemy);
};

#endif // ENEMY_H
