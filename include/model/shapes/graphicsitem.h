#pragma once
#include <vector>
#include <iostream>
class Form;

class GraphicsItem
{
public:

    GraphicsItem();
    GraphicsItem(std::pair<int, int> position);


    //using Pointer = std::shared_ptr<GraphicsItem>;
    virtual ~GraphicsItem() = default;

    virtual void set_bounding_rect(std::vector<double> rect);

    virtual void set_shape(std::vector<std::pair<double, double>> shape);

    virtual void render();

    virtual std::vector<double> get_bounding_rect();
    virtual std::vector<std::pair<double, double>> get_shape();

    virtual bool checkIntersect(const int x, const int y) { return false; };

    virtual void Click() {};

    void change_position(std::pair<int, int> position);
   
    std::pair<int, int> get_position();
    


private:
    std::vector<double> _bounding_rect;
    std::vector<std::pair<double, double>> _shape;
    std::pair<int, int> _position;
};









