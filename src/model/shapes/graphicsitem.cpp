#include "model/shapes/graphicsitem.h"

#include "model/utils/color.h"

GraphicsItem::GraphicsItem() : _bounding_rect{ 0,0,1,1 }, _shape{ {0,0},{0,1},{1,1},{1,0} }, _position{ 0,0 }{};
GraphicsItem::GraphicsItem(std::pair<int, int> position) : _bounding_rect{ 0,0,1,1 }, _shape{ {0,0},{0,1},{1,1},{1,0} }, _position(position) {};


    
void GraphicsItem::set_bounding_rect(std::vector<double> rect) {
    _bounding_rect = rect;
}

void GraphicsItem::set_shape(std::vector<std::pair<double, double>> shape) {
    _shape = shape;
}

void GraphicsItem::render() {
    std::cout << "render";
}

std::vector<double> GraphicsItem::get_bounding_rect() {
    return _bounding_rect;
}

std::vector<std::pair<double, double>> GraphicsItem::get_shape() {
    return _shape;
}

void GraphicsItem::change_position(std::pair<int, int> position)
{
    _position = position;
}

std::pair<int, int> GraphicsItem::get_position()
{
    return _position;
}






