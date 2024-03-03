#include "logger.h"
#include <iostream>
#include <fstream>

Logger::Logger() :stop_(false),break_(false),it_(-1){
	th_logger_ = std::thread(&Logger::write_commands, this);
};

Logger::~Logger() {
	stop();
}



void Logger::stop() {
	stop_ = true;
	condition_.notify_all();
	th_logger_.join();
}

void Logger::write(std::vector<std::string>& vec, std::string& time) {
	const std::lock_guard<std::mutex> guard{ mutex_ };
	tasks_.push(vec);
	if (it_ == -1) { it_ = 0; }
	condition_.notify_all();
};

void Logger::write_commands()
{
	std::ofstream logfile;

	while (!stop_ || !break_)
	{
		std::unique_lock<std::mutex> lock{ mutex_ };
		
		while ((!stop_ || !break_) && (tasks_.empty() && (it_ == block_.size() || it_ == -1)))
		{
			condition_.wait(lock);
		}
		if (stop_&& break_){break;}
		
		if (it_ == block_.size() || it_ == -1)											
		{
			block_ = tasks_.front();												
			it_ = 0;																	
			tasks_.pop();																
													
		}
		lock.unlock();

		std::string sep = "";
		std::cout << "bulk: ";
		while (it_!= block_.size())
		{
			std::cout << sep << block_[it_];
			sep = ", ";
			it_++;
		}
		std::cout << std::endl;
		if (stop_ && tasks_.empty() && (it_ == block_.size()))						
		{break_ = true;	}

	}
}














