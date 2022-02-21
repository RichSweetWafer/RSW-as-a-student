#ifndef TILE_H
#define TILE_H
#include "../Player/player.h"
#include <QJsonObject>
// States:
enum estate {PASS, WALL, START, EXIT};

// Inhabitants:
enum einhabitant {NOBODY, PLAYER, ENEMY, ITEM};

class ITile
{
public:
    virtual estate get_state() = 0;
    virtual void set_state(estate new_state) = 0;
    virtual einhabitant get_inhabitant() = 0;
    virtual void set_object(IObject * obj) = 0;
    virtual IObject * get_object() = 0;
    virtual void set_inhabitant(einhabitant new_inhabitant) = 0;
};

class Tile : public ITile
{
private:
    estate state;
    einhabitant inhabitant;
    IObject * object;
public:
    Tile(estate = PASS);
    Tile(estate stat, einhabitant inhab);
    ~Tile(){};
    void set_state(estate new_state);
    estate get_state();
    void set_inhabitant(einhabitant new_inhabitant);
    einhabitant get_inhabitant();
    void set_object(IObject * obj);
    IObject * get_object();
    friend std::ostream& operator<<(std::ostream& stream, const Tile& tile);
    void write(QJsonObject &json) const;
};


#endif // TILE_H
