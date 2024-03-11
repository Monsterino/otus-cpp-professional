#include <iostream>
#include <string>

#include <server.h>

#include <boost/asio.hpp>

#include <database.h>


int main(int argc, char** argv)
{
    short port;

    if (argc != 2)
    {
        port = 9001;
    }
    else
    {
        port = std::stoi(argv[1]);
    }
    
 //   Database datab;
 /*
    std::cout << datab.execute_command(std::string("INSERT A 0 lean")) << std::endl;
    std::cout << datab.execute_command(std::string("INSERT A 1 sweater")) << std::endl;
    std::cout << datab.execute_command(std::string("INSERT A 2 frank")) << std::endl;
    std::cout << datab.execute_command(std::string("INSERT A 3 violation")) << std::endl;
    std::cout << datab.execute_command(std::string("INSERT A 4 quality")) << std::endl;
    std::cout << datab.execute_command(std::string("INSERT A 5 precision")) << std::endl;

    std::cout << datab.execute_command(std::string("INSERT B 3 proposal")) << std::endl;
    std::cout << datab.execute_command(std::string("INSERT B 4 example")) << std::endl;
    std::cout << datab.execute_command(std::string("INSERT B 5 lake")) << std::endl;
    std::cout << datab.execute_command(std::string("INSERT B 6 flour")) << std::endl;
    std::cout << datab.execute_command(std::string("INSERT B 7 wonder")) << std::endl;
    std::cout << datab.execute_command(std::string("INSERT B 8 selection")) << std::endl;


    std::cout << datab.execute_command(std::string("INTERSECTION asdf")) << std::endl;
    std::cout << datab.execute_command(std::string("SYMMETRIC_DIFFERENCE asdf")) << std::endl;


    std::cout << datab.execute_command(std::string("TRUNCATE A")) << std::endl;
    std::cout << datab.execute_command(std::string("INSERT A 0 shwimsiki")) << std::endl;
    std::cout << datab.execute_command(std::string("INSERT A 0 pimsiki")) << std::endl;
    
    std::cout << datab.execute_command(std::string("INTERSECTION asdf")) << std::endl;
    std::cout << datab.execute_command(std::string("SYMMETRIC_DIFFERENCE asdf")) << std::endl;
*/
    boost::asio::io_context io_context;

    Server server(io_context, port);

    io_context.run();
    
    return 0;

}
