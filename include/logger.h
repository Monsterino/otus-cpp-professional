#pragma once
#include <condition_variable>
#include <atomic>
#include <vector>
#include <fstream>
#include <chrono>
#include <string>
#include <queue>

#include "handler.h"


class Logger : public Handler
{
public:
	Logger()=default;
	~Logger() = default;

	void write(std::vector<std::string> &vec, std::string& time) ;
};


