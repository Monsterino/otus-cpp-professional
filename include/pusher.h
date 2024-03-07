#pragma once
#include <string>
#include <vector>
#include <memory>
#include <shared_mutex>

#include "logger.h"
#include "writer.h"



class Pusher
{
public:
	Pusher(int bulk);
	~Pusher();

	static Pusher * get_Puser(int bulk);
	void push(const std::string& el);
	void push(std::vector<std::string> elements, bool dynamic);
	void execute_block(std::vector<std::string>& elements, std::string time, bool dynamic);

	void disconnect();
	int get_connections();

private:
	inline static int num_connects = 0;
	mutable std::shared_mutex mutex;
	int bulk_;

	std::vector<std::string> command_block_;
	std::string first_command_time_;

	std::vector<std::shared_ptr<Handler>> handlers_;

	void generate_time();


};