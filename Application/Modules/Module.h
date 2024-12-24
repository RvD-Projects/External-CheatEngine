#pragma once

class Module
{
    virtual void Loop() {};
    virtual void Frame() {};

public:
    virtual void Init() {};
    virtual void Render() {};
};
