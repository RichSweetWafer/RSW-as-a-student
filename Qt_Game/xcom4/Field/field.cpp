#include "field.h"


// Constructor
Field::Field(size_t width, size_t height, double chance_to_start_alive,
             size_t life_sim_steps, size_t death_limit, size_t birth_limit)
        :f_width(width), f_height(height), pass_count(0)
{
    tiles.reset(new std::shared_ptr<Tile>[f_height],
                    [](std::shared_ptr<Tile>* ptr) { delete[] ptr; });
    for (size_t i = 0; i < f_height; i++)
    {
        tiles.get()[i].reset(new Tile[f_width], [](Tile* ptr) { delete[] ptr; });
        for (size_t k = 0; k < f_width; k++)
        {
            if (QRandomGenerator::global()->generateDouble() < chance_to_start_alive)
                (*this)[i][k].set_state(WALL);
        }
    }
    (*this)[0][0].set_state(START);
    start.first = 0;
    start.second = 0;
    (*this)[height-1][width-1].set_state(EXIT);
    exit.first = height - 1;
    exit.second = width - 1;
    for (size_t i = 0; i < life_sim_steps; i++)
    {
        tiles = LifeSimulationStep(death_limit, birth_limit);
    }
    for (size_t k = 0, i = 1; k < f_width; k++)
    {
        if (k == f_width/2)
        {
            for (; i < f_height; i++)
                (*this)[i][k].set_state(PASS);
            i-= 2;
        }
        (*this)[i][k].set_state(PASS);
    }
    for (size_t y = 0; y < f_height; y++)
        for (size_t x = 0; x < f_width; x++)
        {
            if ((*this)[y][x].get_state() == PASS)
                pass_count++;
        }
}

std::shared_ptr<std::shared_ptr<Tile>> Field::LifeSimulationStep(size_t death_limit, size_t birth_limit)
{
    std::shared_ptr<std::shared_ptr<Tile>> new_map;
    new_map.reset(new std::shared_ptr<Tile>[f_height],
                    [](std::shared_ptr<Tile>* ptr) { delete[] ptr; });
        for (size_t i = 0; i < f_height; i++)
        {
            new_map.get()[i].reset(new Tile[f_width], [](Tile* ptr) { delete[] ptr; });
            for (size_t k = 0; k < f_width; k++)
            {
                size_t nbs = countAliveNeighbours(k, i);
                if ((*this)[i][k].get_state() == WALL)
                {
                    if (nbs < death_limit)
                        new_map.get()[i].get()[k].set_state(PASS);
                    else
                        new_map.get()[i].get()[k].set_state(WALL);
                }
                else if ((*this)[i][k].get_state() == START)
                    new_map.get()[i].get()[k].set_state(START);
                else if ((*this)[i][k].get_state() == EXIT)
                    new_map.get()[i].get()[k].set_state(EXIT);
                else
                {
                    if (nbs > birth_limit)
                        new_map.get()[i].get()[k].set_state(WALL);
                    else
                        new_map.get()[i].get()[k].set_state(PASS);
                }
            }
        }
    return new_map;
}

size_t Field::countAliveNeighbours(size_t x, size_t y)
{
    size_t count = 0;
    int height = f_height;
    int width = f_width;
    for (int i = -1; i < 2; i++)
        for (int k = -1; k < 2; k++)
        {
            int nx = x + k, ny = y + i;
            if (i == 0 && k == 0)
                continue;
            else if (nx < 0 || ny < 0 || nx >= width || ny >= height)
                count++;
            else if ((*this)[ny][nx].get_state() == WALL)
                count++;
        }
    return count;
}

// Copy constructor
Field::Field(const Field& field)
        :f_width(field.f_width), f_height(field.f_height),
        start(field.start), exit(field.exit) //change tiles(): DONE
{
    tiles.reset(new std::shared_ptr<Tile>[f_height],
                [](std::shared_ptr<Tile>* ptr) { delete[] ptr; });
    for (size_t i = 0; i < f_height; i++)
    {
        tiles.get()[i].reset(new Tile[f_width], [](Tile* ptr) { delete[] ptr; });
        for (size_t k = 0; k < f_width; k++)
        {
            (*this)[i][k] = field[i][k];

        }
    }
    std::cout << "Copy constructor worked here!\n";
}

// Assignment operator
Field& Field::operator=(const Field& field)
{
    if (&field != this)
    {
        f_width= field.f_width;
        f_height = field.f_height;
        start=field.start;
        exit=field.exit;
        tiles = field.tiles;
    }
    std::cout << "Assignment operator worked here!\n";
    return *this;
}

// Move constructor
Field::Field(Field&& field)
{
    std::swap(f_width, field.f_width);
    std::swap(f_height, field.f_height);
    std::swap(start, field.start);
    std::swap(exit, field.exit);
    std::swap(tiles, field.tiles);
    std::cout << "Move constructor worked here!\n";
}

// Move operator
Field& Field::operator=(Field&& field)
{
    if (&field != this)
    {
        std::swap(f_width, field.f_width);
        std::swap(f_height, field.f_height);
        std::swap(start, field.start);
        std::swap(exit, field.exit);
        std::swap(tiles, field.tiles);
    }
    std::cout << "Move operator worked here!\n";
    return *this;
}

Field::~Field(){}

Tile* Field::operator[](const int& index) const
{
    return tiles.get()[index].get();
}

size_t Field::get_height()
{
    return f_height;
}

size_t Field::get_width()
{
    return f_width;
}
