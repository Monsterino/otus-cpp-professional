#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <tuple>
#include <unordered_map>
#include <list>


#include "hashers.h"
#include "file.h"

namespace fs = std::filesystem;


class DFinder {

public:
	DFinder(std::list<fs::path> files, int size_block, std::string hash);
	std::unordered_map<std::size_t, std::list<std::shared_ptr<File>>> get_dublicates();
	void show_dublicates();
	std::string get_block(std::ifstream& stream);
private:
	void check_dublicates();

	int size_block_;
	std::unique_ptr<Hasher> hasher;

	std::unordered_map<std::size_t, std::list<std::shared_ptr<File>>> map_files_;

	void map_cleaner(std::unordered_map<std::size_t, std::list<std::shared_ptr<File>>>& map_files);
	void map_adder(std::size_t key, std::shared_ptr<File> value, std::unordered_map<std::size_t, std::list<std::shared_ptr<File>>>& map);
};


