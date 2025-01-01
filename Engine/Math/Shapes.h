#pragma once

#include "Vector.h"

namespace Shapes
{
    struct Line
    {
        Position p1;
        Position p2;
    };

    struct Rectangle
    {
        Position p;
        Dimension d;
    };

    class Circle
    {
    public:
        Position p;
        Dimension d;
        float radius;
        float diameter;

        Circle(const Position &p, const float radius) : p(p), radius(radius)
        {
            d = Dimension{radius * 2, radius * 2};
            diameter = radius * 2;
        }
    };
}