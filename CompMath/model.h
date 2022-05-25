#ifndef MODEL_H
#define MODEL_H

#include "axis.h"

class Process : public RenderedObject
{
    // Figure and AABB tree
    Point a;
    Point b;
    Point c;
    Point d;
    Polyhedron polyhedron;
    Ray ray;

public:
    Process();

    double f(double ray_angle, double rotate_angle);

    void main();

};

#endif // MODEL_H
