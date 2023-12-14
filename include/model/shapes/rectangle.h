#pragma once
#include "model/utils/color.h"
#include "model/shapes/shape.h"

class Rectangle : public Shape
{
public:
    Rectangle();
    Rectangle(double h,double w);

    Rectangle(std::pair<int, int> position);

    Rectangle(double h, double w, std::pair<int, int> position);

    virtual ~Rectangle() = default;

    virtual void set_height(double h) ;
    virtual void set_width(double w);
    virtual void set_params(double h, double w);

    virtual double get_height();
    virtual double get_width();
    virtual std::pair<double,double> get_params();

    virtual void render() override;
    void Click();

    bool checkIntersect(const int x, const int y) override;

private:
    double _h;
    double _w;
};










