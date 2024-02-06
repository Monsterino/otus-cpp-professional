#include <iostream>

#include "accumulator.h"
#include "logger.h"
#include "writer.h"


int main(int argc, char** argv)
{
    if(argc != 2)
    {
        std::cout << "Uncorrect Bulk size" << std::endl;
        return 1;
    }
    int N = std::stoi(argv[1]);
    auto accumulator = std::make_unique<Accumulator>(N);
    accumulator->add_handler(std::make_shared<Logger>());
    accumulator->add_handler(std::make_shared<Writer>());

    while (true)
    {
        std::string cmd;
        std::cin >> cmd;
        accumulator->add_command(cmd);
        if (cmd == "EOF")
        {
            break;
        }
    }

    return 0;
}

