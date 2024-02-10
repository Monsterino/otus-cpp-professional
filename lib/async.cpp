#include "async.h"

namespace async {

 /*   std::shared_ptr<Context> connect(std::size_t bulk) {
//        return std::unique_ptr<Context>(new Context(bulk));
       // return std::shared_ptr<Context>(new Context(bulk));
        return std::make_shared<Context>(bulk);
    }

    void receive(std::shared_ptr<Context> handle, const char* data, std::size_t size) {
        handle->receive(data, size);
    }

    void disconnect(std::shared_ptr<Context> handle) {
        handle.reset();
    }*/


       Context* connect(std::size_t bulk) {
        return new Context(bulk);
       
    }

    void receive(Context* handle, const char* data, std::size_t size) {
        handle->receive(data, size);
    }

    void disconnect(Context* handle) {
        delete handle;
    }
}