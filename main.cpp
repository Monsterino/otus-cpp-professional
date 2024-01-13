#include <iostream>

#include "handlers.h"
#include "command_executor.h"


int main(int argc, char** argv)
{
    if(argc != 2)
    {
        std::cout << "Uncorrect Bulk size" << std::endl;
        return 1;
    }
    int N = std::stoi(argv[1]);
    Bulk_Accumulator accumulator = Bulk_Accumulator(N);

    Command_Executor executor = { new MainHandler(N,accumulator), new LoggerHandler(accumulator), new ExecuteHandler(accumulator) };

    while (true)
    {
        std::string cmd;
        std::cin >> cmd;
        executor.pass(cmd);
        if (cmd == "EOF")
        {
            break;
        }
    }

    return 0;
}

