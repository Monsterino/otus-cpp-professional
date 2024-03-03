#ifndef ASYNC_H
#define ASYNC_H

#include "context.h"
#include <cstddef>

namespace async {

//std::shared_ptr<Context> connect(std::size_t bulk);
//void receive(std::shared_ptr<Context> handle, const char *data, std::size_t size);
//void disconnect(std::shared_ptr< Context> handle);

Context* connect(std::size_t bulk);
void receive(Context* handle, const char* data, std::size_t size);
void disconnect(Context* handle);
}


#endif