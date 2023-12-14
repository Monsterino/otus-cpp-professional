#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>

class GraphicsItem;


class Document
{
public:
	Document(std::string name);
	~Document() = default;

	void Add_element(std::shared_ptr<GraphicsItem> element);

//	void Set_active(std::shared_ptr<GraphicsItem> element);

	

	std::string _document_name;
//	std::shared_ptr<GraphicsItem> _active_elenent;
	std::vector<std::shared_ptr<GraphicsItem>> _graphicsitems;
};











