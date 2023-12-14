#include "model/shapes/circle.h"

#include <cmath>

Circle::Circle() :_radius(1){};
    
Circle::Circle(double radius) : _radius(radius) {};

Circle::Circle(std::pair<int, int> position) :_radius(1) {
    change_position(position);
};

Circle::Circle(double radius, std::pair<int, int> position) : _radius(radius) {
    change_position(position);
};

   

void Circle::set_radius(double radius) {
    _radius = radius;
}

double Circle::get_radius() {
    return _radius;
}

bool Circle::checkIntersect(const int x, const int y) {

    auto position = get_position();
    if ( std::pow((double(x)- position.first),2) + std::pow((double(y) - position.second),2) <= std::pow(_radius,2)) {
        return true;
    }
    else
    {
        return false;
    } 
}

void Circle::Click(){
    render();
};

void Circle::render() {
    std::cout << "Render: Circle; Color is: ";
    get_color();
    std::cout << " Radius: " << _radius << std::endl;
}










