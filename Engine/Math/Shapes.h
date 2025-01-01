#pragma once

#include "Vector.h"

namespace Shapes
{
    struct Box
    {
        Position pStart;
        Dimension d;
        Position pStop = {pStart.x + d.w, pStart.y + d.h};
        GuiColor color;
        GuiColor borderColor;
    };

    struct Line
    {
        Position pStart;
        Position pEnd;
        Dimension d = {pEnd.x - pStart.x, pEnd.y - pStart.y};
        GuiColor color;
        GuiColor borderColor;
    };

    struct Circle
    {
        Position p;
        float r = 1;
        Dimension d = {r * 2, r * 2};
        GuiColor color;
        GuiColor borderColor;
    };
}