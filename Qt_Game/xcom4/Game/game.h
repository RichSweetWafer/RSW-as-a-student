#ifndef GAME_H
#define GAME_H
#include <cstdlib>
#include <QTime>
#include <QThread>
#include "../Regulator/regulator.h"

template<class T>
class Presenter;

class Logger;

class Game
{
private:
    Field field;
    Player player;
    std::vector<Enemy> enemies;
    size_t enemy_count;
    std::vector<Item> items;
    size_t item_count;


public:
    Game(size_t enemies, size_t items);
    ~Game(){};

    template<class T> friend class Presenter;
    friend class Logger;

    void buff_player(eeffect buff, size_t mp);
    void player_attack();

    void place_items(size_t it_count);
    void place_enemies(size_t en_count);

    void enemy_turn();
    void enemy_attack(size_t damage);

    void movement(edirection dir);
};

#endif // GAME_H
