#include "model/shapes/shape.h"


Shape::Shape() :_shape_color(Color(0, 0, 0, 255)) {};
Shape::Shape(Color color) : _shape_color(color) {};


Color Shape::get_color() {
    _shape_color.get_color();
    return _shape_color;
}

void Shape::set_color(Color color) {
    _shape_color = color;
}

void Shape::render() {
    std::cout << "Render: Shape: color is:";
    _shape_color.get_color();
}










