#ifndef PLAYER_H
#define PLAYER_H
#include "../Enemy/enemy.h"
#include <QKeyEvent>

enum edirection {UP, DOWN, LEFT, RIGHT};

class IPlayer
{
public:
    virtual void set_strength(size_t player_strength) = 0;
    virtual size_t get_strength() = 0;
    virtual edirection get_direction() = 0;
    virtual void set_direction(edirection dir) = 0;
};

class Player : public IPlayer, public Object
{
private:
    size_t strength; // spell cast damage
    edirection direction;
public:
    Player(size_t health, size_t player_strength,
           size_t mana, QPair<size_t, size_t> pos);
    Player(QPair<size_t, size_t> pos);
    void set_strength(size_t player_strength);
    size_t get_strength();
    void set_direction(edirection dir);
    edirection get_direction();
    friend std::ostream& operator<<(std::ostream& stream, const Player& player);
};

#endif // PLAYER_H
