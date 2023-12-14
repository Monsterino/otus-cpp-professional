#include "model/shapes/rectangle.h"

Rectangle::Rectangle() :_h(1), _w(1) {};

Rectangle::Rectangle(double h,double w) : _h(h), _w(w) {};

Rectangle::Rectangle(std::pair<int, int> position) :_h(1), _w(1) { change_position(position); };

Rectangle::Rectangle(double h, double w, std::pair<int, int> position) : _h(h), _w(w) { change_position(position); };


void Rectangle::set_height(double h) {
    _h = h;
}

void Rectangle::set_width(double w) {
    _w = w;
}

void Rectangle::set_params(double h, double w) {
    _h = h;
    _w = w;
}

double Rectangle::get_height() {
    return _h;
}

double Rectangle::get_width() {
    return _w;
}

std::pair<double,double> Rectangle::get_params() {
    return { _h,_w };
}

bool Rectangle::checkIntersect(const int x, const int y) 
{
    auto position = get_position();
    int x_local = x - position.first;
    int y_local = y - position.second;


    if ((x_local >= 0) && (x_local <= _w) && (y_local >= 0) && (y_local <= _h)) {
        return true;
    }
    else
    {
        return false;
    }
};

void Rectangle::Click() {
    render();
};




void Rectangle::render() {
    std::cout << "Render: Rectangle; Color is: ";
    get_color();
    std::cout << " Parameters: height: " << _h << ", width : " << _h << std::endl;
 
}










