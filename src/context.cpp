#include "context.h"

Context::Context(std::size_t bulk)
{
	accumulator_ = std::shared_ptr<Accumulator>(new Accumulator(bulk));
	parser_ = std::shared_ptr<Parser>(new Parser(accumulator_));
	
}




void Context::receive(const char* data, std::size_t size) 
{
	parser_->parse(data, size);
}