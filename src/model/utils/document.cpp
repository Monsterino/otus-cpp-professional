#include "model/utils/document.h"
#include "model/shapes/graphicsitem.h"
#include "view/view.h"
#include "controller/controller.h"



Document::Document(std::string name) : _document_name(name){};



void Document::Add_element(std::shared_ptr<GraphicsItem> element) {
	_graphicsitems.push_back(element);
}

//void Document::Set_active(std::shared_ptr<GraphicsItem> element ){
//	_active_elenent = element;
//}









