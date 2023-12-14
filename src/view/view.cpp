#include "view/view.h"
#include "controller/controller.h"
#include "model/shapes/graphicscene.h"

#include <memory>
#include <algorithm>


FormItem::FormItem(Form* parent) :parent(parent) {};

void FormItem::Click(const int x, const int y) {
    std::cout << "FormItem" << std::endl;
};



ElementSize::ElementSize(int x_0, int y_0, int width, int height) : _x_0(x_0), _y_0(y_0),width(width),height(height){};

bool ElementSize::CheckPoint(const int x, const int y) {
    if (x >= _x_0 && x <= (_x_0 + width) && y >= _y_0 && y <= (_y_0 + height)) {
        return true;
    }
    else
    {
        return false;
    }
}

int ElementSize::get_x0() {
    return _x_0;
};
int ElementSize::get_y0() {
    return _y_0;
};


MenuItem::MenuItem(std::string name, std::string tooltip, Form* activeForm) : _name(name), _tooltip(tooltip), FormItem(activeForm){};
void MenuItem::Click() {
    std::cout << "MenuItem" << std::endl;
};




Menu::Menu(std::string name, std::string tooltip, Form* parent) : _name(name), _tooltip(tooltip),FormItem(parent) {};
Menu::Menu(Form* parent) : _name("Menu"), _tooltip("Menu"), FormItem(parent) {};
Menu::~Menu() {    }

void Menu::AddItem(std::shared_ptr<MenuItem> item, ElementSize position)
{
    _items.push_back(std::pair{ item ,position });
}
void Menu::Click(const int x, const int y) 
{
    for (auto item : _items)
    {
        if (item.second.CheckPoint(x, y)) {
            item.first->Click();
            break;
        }
    }
}

 


Submenu::Submenu(std::string name, std::string tooltip, Form* parent) :Menu(name, tooltip,parent) {};
Submenu::Submenu(Form* parent) :Menu("SubMenu", "SubMenu", parent) {};





NewItem::NewItem(std::string name, std::string tooltip, Form* activeForm) : MenuItem{ name ,tooltip, activeForm } {};
void NewItem::Click() {

    std::cout << "======================" << std::endl;
    std::cout << "New document clicked" << std::endl;
    parent->_editor->Create_new_document();
       
}




ExportItem::ExportItem(std::string name, std::string tooltip, Form* activeForm) : MenuItem{ name ,tooltip, activeForm } {};
void ExportItem::Click() { 
    std::cout << "======================" << std::endl;
    std::cout << "export clicked" << std::endl;
    std::string path;
    std::cout << "Input export directory" << std::endl;
    std::cin >> path;
    parent->_editor->Export_document(path);
}



ImportItem::ImportItem(std::string name, std::string tooltip, Form* activeForm) : MenuItem{ name ,tooltip,activeForm }  {};
void ImportItem::Click() {
    std::cout << "Import item clicked" << std::endl;
    std::string path;
    std::cout << "Input import directory" << std::endl;
    std::cin >> path;
    parent->_editor->Import_document(path);
}



AddItem::AddItem(std::string name, std::string tooltip, Form* activeForm) : MenuItem{ name ,tooltip,activeForm } {};
void AddItem::Click() {
    std::cout << "Add item clicked" << std::endl;
    std::cout << "======================" << std::endl;
    std::cout << "Choose element" << std::endl;
    std::cout << "0: Circle" << std::endl << "1: Rectangle" << std::endl;
    short choice;
    int x, y;
    
    std::cin >> choice;

    std::cout << "Input scene local position coordinates" << std::endl;
    std::cin >> x >> y;

    auto position = std::pair<int, int>{ x,y };
    std::shared_ptr<GraphicsItem> item(new GraphicsItem());

    switch (choice)
    {
    case 0:
    {
        std::cout << "======================" << std::endl;
        std::cout << "Circle" << std::endl;
        std::cout << "Input radius" << std::endl;
        int radius;
        std::cin >> radius;
        item.reset(new Circle(radius, position));
        std::cout << "======================" << std::endl;
        break;
    }
    case 1:
    {
        std::cout << "======================" << std::endl;
        std::cout << "Rectangle" << std::endl;
        std::cout << "Input Width and Height" << std::endl;
        int w,h;
        std::cin >> w>>h;
        item.reset(new Rectangle(w, h, position));
        std::cout << "======================" << std::endl;
        break;
    }
    default:
        break;
    }

    //std::shared_ptr<GraphicsItem> circle(new Circle(3, position));
    //std::shared_ptr<GraphicsItem> rectangle(new Rectangle(10,10, position));

    parent->_editor->Add_element(item);
}


DeleteItem::DeleteItem(std::string name, std::string tooltip, Form* activeForm) : MenuItem{ name ,tooltip, activeForm } {};
void  DeleteItem::Click() { std::cout << "Delete item clicked" << std::endl; }





Form::Form(Editor* editor): _editor(editor)
{

    std::shared_ptr<Menu>mainMenu(new Menu(this)) ;
    mainMenu->AddItem(std::shared_ptr<NewItem>(new NewItem("New Document", "Create new document",this)), ElementSize(0, 0, 99, 9));
    mainMenu->AddItem(std::shared_ptr<ExportItem>(new ExportItem("Export", "Export document", this)), ElementSize(0, 10, 99, 9));
    mainMenu->AddItem(std::shared_ptr<ImportItem>(new ImportItem("Import", "Import document", this)), ElementSize(0, 20, 99, 9));
    AddFormItem(mainMenu, ElementSize(0, 0, 100, 29));

    std::shared_ptr<Submenu> submenu(new Submenu(this));
    submenu->AddItem(std::shared_ptr<AddItem>(new AddItem("Add Shape", "Add new shape", this)), ElementSize(100, 0, 990, 9));
    submenu->AddItem(std::shared_ptr<DeleteItem>(new DeleteItem("Delete Shape", "Delete shape", this)), ElementSize(100, 10, 99, 9));
        
    AddFormItem(submenu, ElementSize(101, 0, 100, 19));

    std::shared_ptr<GraphicsScene> scene(new GraphicsScene(ElementSize(0,30,1000,1000),this));
    AddFormItem(scene, ElementSize(0, 30, 1000, 1000));
     

};

Form::~Form() {};

void Form::AddFormItem(std::shared_ptr<FormItem> item, ElementSize position) {
    all_items.push_back(std::pair{item, position});
}

void Form::initialize_scene(GraphicsScene* scene) {
    main_scene.reset(scene);
    _editor->initialize_scene(main_scene);
}

void Form::Click(const int x, const int y) {
    for (auto item : all_items)
    {
        if (item.second.CheckPoint(x, y)) {
            item.first->Click(x, y);
            break;
        }
    }
}
