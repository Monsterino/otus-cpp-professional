#pragma once

#include "model/shapes/circle.h"
#include "model/shapes/rectangle.h"

#include <string>
#include <set>
#include <iostream>
#include <memory>

class Form;
class Editor;
class GraphicsScene;



class FormItem
{
public:
    FormItem(Form* parent);
    virtual ~FormItem() = default;
    virtual void Click(const int x, const int y);
    Form* parent;
};

class ElementSize
{
public:
    ElementSize(int x_0, int y_0, int width, int height);
    ~ElementSize() = default;
    bool CheckPoint(const int x, const int y);
    int get_x0();
    int get_y0();
private:
    int _x_0;
    int _y_0;
    int width;
    int height;
};


class MenuItem : public FormItem
{
public:
    MenuItem(std::string name, std::string tooltip,  Form* activeForm);
    virtual void Click();

    std::string _name;
    std::string _tooltip;

};

class Menu : public FormItem
{
public:
    Menu(std::string name, std::string tooltip, Form* parent);
    Menu(Form* parent);
    virtual ~Menu();

    void AddItem(std::shared_ptr<MenuItem> item, ElementSize position);
    virtual void Click(const int x, const int y) override;

    std::string _name;
    std::string _tooltip;

    std::vector<std::pair<std::shared_ptr<MenuItem>, ElementSize>> _items;
};

class Submenu : public Menu
{
public:
    Submenu(std::string name, std::string tooltip, Form* parent);
    Submenu(Form* parent);
};



class NewItem : public MenuItem
{
public:
    NewItem(std::string name, std::string tooltip, Form* activeForm);
    void Click() override;
};

class ExportItem : public MenuItem
{
public:
    ExportItem(std::string name, std::string tooltip, Form* activeForm);
    void Click() override;
};

class ImportItem : public MenuItem
{
public:
    ImportItem(std::string name, std::string tooltip, Form* activeForm);
    void Click() override;
};

class AddItem : public MenuItem
{
public:
    AddItem(std::string name, std::string tooltip, Form* activeForm);
    void Click() override;
};

class DeleteItem : public MenuItem
{
public:
    DeleteItem(std::string name, std::string tooltip, Form* activeForm);
    void Click() override;
};


class Form
{
public:
    
    void AddFormItem(std::shared_ptr<FormItem> item, ElementSize position);

    Form(Editor* editor);
    ~Form();

    void Click(const int x, const int y);

    void initialize_scene(GraphicsScene* scene);

    std::vector<std::pair<std::shared_ptr<FormItem>, ElementSize>> all_items;
    
    
    Editor* _editor;
    std::shared_ptr<GraphicsScene> main_scene;

    



};












