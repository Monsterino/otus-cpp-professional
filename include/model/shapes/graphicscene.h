#pragma once
#include <vector>
#include <iostream>
#include <map>

class GraphicsItem;
#include "view/view.h"

//class FormItem;




class GraphicsScene : public FormItem
{
public:
    GraphicsScene(ElementSize, Form* parent);

    virtual ~GraphicsScene() = default;
    virtual void Click(const int x, const int y) override;

    void Clear();

    //void add_element(std::pair<double, double> position, GraphicsItem::Pointer item);

    void add_element(std::shared_ptr<GraphicsItem> item);

    std::pair<double, double> GetLocalCoordinates(const int x, const int y);
 

private:
    ElementSize _size;
    std::vector<std::shared_ptr<GraphicsItem>> _elements;

};









