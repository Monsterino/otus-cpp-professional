#include "model/utils/color.h"



Color::Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) :_r(r), _g(g), _b(b), _a(a) {}

void Color::get_color() {
  std::cout << static_cast<int>(_r) << ","
            << static_cast<int>(_g) << ","
            << static_cast<int>(_b) << ","
            << static_cast<int>(_a);
}


inline std::ostream& operator<<(std::ostream& stream, Color color)
{
    stream << "Color{ "
        << static_cast<int>(color._r) << ", "
        << static_cast<int>(color._g) << ", "
        << static_cast<int>(color._b) << ", "
        << static_cast<int>(color._a) << " }";
    return stream;
}









