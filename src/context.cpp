#include "context.h"

Context::Context(std::size_t bulk)
{
	static int id = 0;
	id_ = id++;
	accumulator_ = std::shared_ptr<Accumulator>(new Accumulator(bulk));
	parser_ = std::shared_ptr<Parser>(new Parser(accumulator_));
	logger_ = std::shared_ptr<Logger>(new Logger());
	writer_ = std::shared_ptr<Writer>(new Writer(id));


	accumulator_->add_handler(logger_);
	accumulator_->add_handler(writer_);
	
	
}

void Context::receive(const char* data, std::size_t size) 
{
	parser_->parse(data, size);
}