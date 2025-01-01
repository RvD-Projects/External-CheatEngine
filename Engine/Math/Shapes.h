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
        float radius = 1;
        Dimension d = {radius * 2, radius * 2};
        GuiColor color;
        GuiColor borderColor;
    };

    struct Triangle
    {
        Position p1;
        Position p2;
        Position p3;
        GuiColor color;
        GuiColor borderColor;
    };

    bool InterSects(const Line &line1, const Line &line2) noexcept
    {
        const auto x1 = line1.pEnd.x - line1.pStart.x;
        const auto y1 = line1.pEnd.y - line1.pStart.y;
        const auto x2 = line2.pEnd.x - line2.pStart.x;
        const auto y2 = line2.pEnd.y - line2.pStart.y;

        const auto d = (-x2 * y1 + x1 * y2);

        if (d == 0)
            return false;

        const auto s = (-y1 * (line1.pStart.x - line2.pStart.x) + x1 * (line1.pStart.y - line2.pStart.y)) / d;
        const auto t = (x2 * (line1.pStart.y - line2.pStart.y) - y2 * (line1.pStart.x - line2.pStart.x)) / d;

        return s >= 0 && s <= 1 && t >= 0 && t <= 1;
    };

    bool InterSects(const Box &box1, const Box &box2) noexcept
    {
        return box1.pStart.x < box2.pStop.x && box1.pStop.x > box2.pStart.x &&
               box1.pStart.y < box2.pStop.y && box1.pStop.y > box2.pStart.y;
    };

    bool InterSects(const Box &box, const Line &line) noexcept
    {
        const auto x = max(line.pStart.x, min(box.pStart.x, line.pEnd.x));
        const auto y = max(line.pStart.y, min(box.pStart.y, line.pEnd.y));

        return x == line.pStart.x && y == line.pStart.y;
    };

    bool InterSects(const Box &box, const Circle &circle) noexcept
    {
        const auto x = max(box.pStart.x, min(circle.p.x, box.pStop.x));
        const auto y = max(box.pStart.y, min(circle.p.y, box.pStop.y));

        const auto dx = x - circle.p.x;
        const auto dy = y - circle.p.y;

        return (dx * dx + dy * dy) <= (circle.radius * circle.radius);
    };
}