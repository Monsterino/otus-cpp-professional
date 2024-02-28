#include "async.h"

namespace async {
     
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