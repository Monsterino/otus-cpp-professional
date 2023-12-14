#include "controller/controller.h"
#include "model/shapes/graphicscene.h"


Editor::Editor():currentForm(this) {
	std::cout << "Editor  Created" << std::endl;
	Create_new_document();
};
	
void Editor::Create_new_document()
{
	if (ActiveDocument)
	{
		std::cout << "New document created. Old document deleted" << std::endl;
	}
	ActiveDocument.reset(new Document("New document"));
	main_scene->Clear();
};
	

void Editor::Export_document(std::string path) { std::cout << "Document saved to: " << path << std::endl; };
void Editor::Import_document(std::string path) { std::cout << "Import document from: " << path << std::endl; };

void Editor::Add_element(std::shared_ptr<GraphicsItem> item) {
	std::cout << "We are in the Edittors add_element function" << std::endl;
	ActiveDocument->Add_element(item);
	main_scene->add_element(item);
};

void Editor::initialize_scene(std::shared_ptr<GraphicsScene> scene) {
	main_scene = scene;
};

void Editor::Select_Item(std::shared_ptr<GraphicsItem> item) {
	selectedItem = item;
	std::cout << "active item is: ";
	item->render();
};

void Editor::Delete_active_element() {};
void Editor::Click(int x, int y) {
	currentForm.Click(x,y);
};












