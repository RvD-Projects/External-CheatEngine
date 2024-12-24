#pragma once

#include <cstdint>
#include <vector>

class CheatModule
{
    struct ViewMatrix
    {
        float m[16];
    };

    uintptr_t PID;
    uintptr_t CLIENT;
    ViewMatrix vm = {};
    std::vector<uintptr_t> entities;

    void Loop();
    void Frame();
    void Render();
};

