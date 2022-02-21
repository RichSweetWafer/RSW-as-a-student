    #include "object.h"
Object::Object(etype obj_type, size_t health, size_t mana,
               QPair<size_t, size_t> pos)
{
    type = obj_type;
    hp = health;
    mp = mana;
    position = pos;
}


void Object::set_position(size_t y, size_t x)
{
    position.first = y;
    position.second = x;
}

QPair<size_t, size_t> Object::get_position()
{
    return position;
}

void Object::set_hp(int health)
{
    hp = health;
}

int Object::get_hp()
{
    return hp;
}

void Object::set_mp(size_t mana)
{
    mp = mana;
}

size_t Object::get_mp()
{
    return mp;
}

etype Object::get_type()
{
    return type;
}
