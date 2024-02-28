#include <iostream>
#include <string>
#include <cstring>

#include "async.h"


int main() {
   
    std::cout << "Enter block size" << std::endl;
    int block_size;
    std::cin >> block_size;
    
    auto context = async::connect(5);

    while (true)
    {
        std::string cmd;
        std::cin >> cmd;
        async::receive(context, cmd.data(), cmd.length());
        if (cmd == "EOF")
        {
            break;
        }
    }

    return 0;

}
