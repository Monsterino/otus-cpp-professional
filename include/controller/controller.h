#pragma once
#include <iostream>
#include <memory>

#include "view/view.h"
#include "model/utils/document.h"



//class Form;
class Document;

class Editor
{
public:
	std::unique_ptr<Document> ActiveDocument;
	std::shared_ptr<GraphicsItem> selectedItem;
	std::shared_ptr<GraphicsScene> main_scene;

	Editor();
	
	void Create_new_document();
	void Export_document(std::string path);
	void Import_document(std::string path);

	void Add_element(std::shared_ptr<GraphicsItem> item);

	void Delete_active_element();
	void Click(int x, int y);
	void initialize_scene(std::shared_ptr<GraphicsScene> scene);

	void Select_Item(std::shared_ptr<GraphicsItem> item);
	Form currentForm;
	
	

};











