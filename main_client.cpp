#include <iostream>
#include <string>

#include <server.h>

#include <boost/asio.hpp>




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

    using boost::asio::ip::tcp;
    boost::asio::io_context io_context;

    tcp::socket socket(io_context);
    tcp::resolver resolver(io_context);

    //socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), port));
   

    std::string command;
    try
    {
        boost::asio::connect(socket, resolver.resolve("127.0.0.1", std::to_string(port)));
        while (true)
        {
            char buffer[1024];
            boost::system::error_code ec;
            std::getline(std::cin, command);
            command += "\n";



            auto result = boost::asio::write(socket, boost::asio::buffer(command.c_str(), command.size()));
            if (ec)
            {
                std::cout << "Connection or other error. Break" << std::endl;
                break;
            }


            size_t size = socket.read_some(boost::asio::buffer(buffer), ec);
            std::cout << std::string{ buffer ,size };
        }
    }
    catch (const std::exception&)
    {
        std::cout << "Connection or other error. Break" << std::endl;
//        socket.close();
    }
    socket.close();
   
    
    return 0;

}
