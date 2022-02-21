#ifndef FIELD_H
#define FIELD_H
#include "../Tile/tile.h"
#include <utility>
#include <QRandomGenerator>

template<class T>
class Presenter;


class Field
{
private:
    size_t f_width;
    size_t f_height;
    // size_t wall_count;
    std::shared_ptr<std::shared_ptr<Tile>> tiles;
    std::pair <size_t,size_t> start;
    std::pair <size_t,size_t> exit;

    std::shared_ptr<std::shared_ptr<Tile>> LifeSimulationStep(size_t death_limit, size_t birth_limit);
public:
    size_t pass_count;

    Field(size_t width = 20, size_t height = 20, double chance_to_start_alive = 0.4,
          size_t life_sim_steps = 2, size_t death_limit = 3, size_t birth_limit = 4);
    Field(const Field& field);
    Field(Field&& field);
    ~Field();

    Tile* operator[](const int& index) const;
    Field& operator=(const Field& field);
    Field& operator=(Field&& field);
    size_t get_height();
    size_t get_width();
    size_t countAliveNeighbours(size_t x, size_t y);
};


#endif // FIELD_H
