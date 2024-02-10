#pragma once
#include <condition_variable>
#include <atomic>
#include <vector>
#include <fstream>
#include <chrono>
#include <string>
#include <queue>
#include <thread>
		

#include "handler.h"


class Logger : public Handler
{
public:
	Logger();
	~Logger();

	void write_commands();


	void write(std::vector<std::string> &vec, std::string& time) override;
	void stop();

private:
	std::condition_variable condition_;
	std::atomic<bool> stop_;
	bool break_;
	std::mutex mutex_;
	std::thread th_logger_;
	std::vector<std::string> block_;
	std::queue<std::vector<std::string>> tasks_;
	int it_;
};


