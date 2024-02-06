#pragma once
#include <condition_variable>
#include <atomic>
#include <vector>
#include <fstream>
#include <chrono>
#include <string>
#include <queue>

#include "handler.h"



class Writer : public Handler
{
public:
	Writer() = default;
	~Writer() = default;
	void write(std::vector<std::string> &vec,std::string& time) ;

};


