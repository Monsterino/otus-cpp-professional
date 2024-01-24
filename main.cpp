#include <iostream>

#include <boost/program_options.hpp>

#include "file_list_creator.h"
#include "dublicate_finder.h"

namespace po = boost::program_options;


void set_bulk(size_t bulk) {
    std::cout << "bulk size is " << bulk << std::endl;
}

int main(int argc, const char *argv[]) {

    po::options_description desc{ "Options" };

    desc.add_options()
        ("help,h", "This screen")
        ("directory", po::value<std::vector<std::string>>()->required()->multitoken(), "Directories to search duplicates")
        ("exclude_directory", po::value<std::vector<std::string>>()->multitoken(), "Direcroties to exclude from search")
        ("size_file", po::value<int>()->default_value(1), "File size")
        ("scan_level", po::value<bool>()->default_value(true), "Recursive subdirectory scaning")
        ("size_block", po::value<int>()->required()->default_value(4096), "Block size")
        ("hash", po::value<std::string>()->default_value("crc32"), "Hash alghoritm")
        ("mask_include", po::value<std::vector<std::string>>(), "Include only files corresponding to these masks in search")
        ("mask_exclude", po::value<std::vector<std::string>>(), "Exclude files corresponding to these masks from search");

    po::variables_map vm;
    try {
        po::store(parse_command_line(argc, argv, desc), vm);

        if (vm.count("help")!=0) {
            std::cout << desc << '\n';
        }
        po::notify(vm);

    }

    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    if (vm.count("size_file") && vm["size_file"].as<int>() < 0)
    {
        std::cout << "size_file must be more or equal than 0" << std::endl;
        return 1;
    }

    if (vm.count("size_block") && vm["size_block"].as<int>() <= 0)
    {
        std::cout << "size_file must be more than 0" << std::endl;
        return 1;
    }
    

    auto directory_list = vm.count("directory")
                            ? vm["directory"].as<std::vector<std::string>>() 
                            : std::vector<std::string>();
    
    auto exclude_directory = vm.count("exclude_directory")
                            ? vm["exclude_directory"].as<std::vector<std::string>>()
                            : std::vector<std::string>();

    auto mask_include = vm.count("mask_include")
                            ? vm["mask_include"].as<std::vector<std::string>>()
                            : std::vector<std::string>();

    auto mask_exclude = vm.count("mask_exclude")
                            ? vm["mask_exclude"].as<std::vector<std::string>>()
                            : std::vector<std::string>();
   
    int size_file = vm["size_file"].as<int>();
    int size_block = vm["size_block"].as<int>();
    std::string hash = vm["hash"].as<std::string>();
    bool scan_level = vm["scan_level"].as<bool>();



    DirList dir = DirList(directory_list, exclude_directory, mask_include, mask_exclude, size_file, scan_level);

    auto dirs = dir.getDirs();
   
    DFinder finder = DFinder(dirs, size_block, hash);

    finder.show_dublicates();
};

