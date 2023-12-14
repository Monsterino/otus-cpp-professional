#pragma once
#include <iostream>

struct Color
{
public:
    Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);

    ~Color() = default;
    void get_color();


std::uint8_t _r;
std::uint8_t _g;
std::uint8_t _b;
std::uint8_t _a;
};







