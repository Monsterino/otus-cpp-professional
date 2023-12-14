#pragma once
#include "model/utils/color.h"
#include "model/shapes/shape.h"

class Circle: public Shape
{
public:
    Circle();
    Circle(double radius);
    Circle(std::pair<int, int> position);
    Circle(double radius, std::pair<int, int> position);

    virtual ~Circle() = default;

    virtual void set_radius(double radius);
    virtual double get_radius();

    bool checkIntersect(const int x, const int y) override;

    virtual void render() override;

    void Click();

private:
    double _radius;
};









