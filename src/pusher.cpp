#include "pusher.h"
#include <chrono>


Pusher::Pusher(int bulk) : bulk_(bulk) {
	handlers_.push_back(std::shared_ptr<Logger>(new Logger()));
	handlers_.push_back(std::shared_ptr<Writer>(new Writer()));


}

Pusher::~Pusher() {
	execute_block(command_block_, first_command_time_,false);
}



Pusher* Pusher::get_Puser(int bulk) {
	static Pusher pusher(bulk);
	num_connects++;
	return &pusher;
}

void Pusher::push(const std::string& el)
{
	if (el!="EOF") {
		generate_time();
		std::unique_lock<std::shared_mutex> lk(mutex);
		command_block_.push_back(el);
		lk.unlock();
		if (command_block_.size() == bulk_)
		{
			execute_block(command_block_, first_command_time_,false);
		}
	}
	else {
		execute_block(command_block_, first_command_time_,false);
	}

}
void Pusher::push(std::vector<std::string> elements, bool dynamic) 
{
	
	if (dynamic)
	{
		std::string time = std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
		if(!command_block_.empty()){
			execute_block(command_block_, first_command_time_,false);
		}
		execute_block(elements, time, true);
	}
	else
	{
		for (auto element : elements)
		{
			push(element);
		}
	}

}


void Pusher::execute_block(std::vector<std::string>& elements, std::string time,bool dynamic = false) {

	if (!elements.empty())
	{
		for (auto handler : handlers_)
		{
			handler->write(elements, time);
		}
	}
	elements.clear();

	
}


void Pusher::generate_time() {

	if (!command_block_.size())
	{
		auto now = std::chrono::system_clock::now();
		first_command_time_ = std::to_string(std::chrono::system_clock::to_time_t(now));
	}

}


void Pusher::disconnect() {
	num_connects--;
}

int Pusher::get_connections() {
	return num_connects;
}