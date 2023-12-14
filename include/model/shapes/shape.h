#pragma once
#include "model/utils/color.h"
#include "graphicsitem.h"

class Shape: public GraphicsItem
{
public:
    Shape();
    Shape(Color color);
    virtual ~Shape() = default;

    virtual Color get_color();

    virtual void set_color(Color color);

    virtual void render() override;

    virtual bool checkIntersect(const int x, const int y) { return false; };
    virtual void Click() {};
private:
    Color _shape_color;
};









