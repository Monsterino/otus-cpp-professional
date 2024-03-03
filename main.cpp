#include <iostream>
#include <string>

#include <async.h>
#include <server.h>

#include <boost/asio.hpp>


int main(int argc, char** argv)
{
    int bulk_size;
    short port;

    if (argc != 3)
    {
        port = 9000;
        bulk_size = 3;
    }
    else
    {
        bulk_size = std::stoi(argv[1]);
        port = std::stoi(argv[2]);
    }
    

    boost::asio::io_context io_context;

    Server server(io_context, port, bulk_size);

    io_context.run();
    
    return 0;

}

