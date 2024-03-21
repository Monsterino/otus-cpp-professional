#include "reader.h"
#include "splitter.h"
#include "functions.h"
#include "mapper.h"
#include "shuffler.h"
#include "reducer.h"



int main(int argc, char** argv) {
    int m_num;
    int r_num;
    std::string path;
    if (argc != 4)
    {
         m_num = 5;
         r_num = 4;
         path = "../test/data.txt";
    }
    else
    {
        path = argv[1];
        m_num = std::stoi(argv[2]);
        r_num = std::stoi(argv[3]);
    }


    std::vector<std::shared_ptr<FileReader>> tmp = splitter(m_num).split(path);
    auto func = pair_of_line();

    Mapper mp =  Mapper(m_num,tmp,func);
    auto result = mp.run();

    Shuffler sh = Shuffler(r_num);
    auto res = sh.run(result);

    Reducer rd = Reducer(r_num);
    rd.run(res);

    return 0;
}
