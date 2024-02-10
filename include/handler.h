#pragma once
#include <iostream>

class Handler
{
public:
	virtual ~Handler() = default;
	virtual void write(std::vector<std::string>& vec, std::string& time)=0;
};


