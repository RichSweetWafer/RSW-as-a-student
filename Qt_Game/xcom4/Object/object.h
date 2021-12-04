#ifndef OBJECT_H
#define OBJECT_H
#include <memory>
#include <QPair>
#include <iostream>

enum etype {OBJ_PLAYER, OBJ_ENEMY, OBJ_ITEM};

class IObject
{
public:
    virtual void set_hp(int health) = 0;
    virtual int get_hp() = 0;
    virtual void set_mp(size_t mana) = 0;
    virtual size_t get_mp() = 0;
    virtual QPair<size_t, size_t> get_position() = 0;
    virtual void set_position(size_t y, size_t x) = 0;
    virtual etype get_type() = 0;
};

class Object : public IObject
{
protected:
    QPair<size_t, size_t> position;
    int hp;
    size_t mp;
    etype type;

public:
    Object(etype obj_type, size_t health, size_t mana,
           QPair<size_t, size_t> pos);
    void set_position(size_t y, size_t x);
    QPair<size_t, size_t> get_position();
    void set_hp(int health);
    int get_hp();
    void set_mp(size_t mana);
    size_t get_mp();
    etype get_type();
};

#endif // OBJECT_H
