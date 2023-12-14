#include "model/shapes/graphicscene.h"
#include "model/shapes/graphicsitem.h"
#include "view/view.h"

#include "controller/controller.h"


GraphicsScene::GraphicsScene(ElementSize size,Form* parent) : _size(size) , FormItem(parent){

    parent->initialize_scene(this);
    std::cout << "initialize scene" << std::endl;
};

    

void GraphicsScene::Click(const int x, const int y)
{ 
    std::cout << "Clicked on Scene" << std::endl;
    auto coordinates = GetLocalCoordinates(x, y);
    for (auto item : _elements)
    {
       if (item->checkIntersect(coordinates.first, coordinates.second)) {
           item->Click();

           parent->_editor->Select_Item(item);
           break;
       }

    }
}

std::pair<double,double> GraphicsScene::GetLocalCoordinates(const int x, const int y) 
{
    return std::pair<int, int >{x - _size.get_x0(), y - _size.get_y0()};
}

void GraphicsScene::Clear() 
{
    _elements.clear();
};



void GraphicsScene::add_element(std::shared_ptr<GraphicsItem> item) {
   _elements.push_back(item);
}






