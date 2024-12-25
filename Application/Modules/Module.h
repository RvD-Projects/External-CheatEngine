#pragma once

class Module
{
    virtual void Frame() {};

public:
    virtual void Init() {};
    virtual void Loop() {};
    virtual void Render() {};
};
