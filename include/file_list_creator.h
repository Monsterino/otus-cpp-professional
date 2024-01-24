#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <list>

namespace fs = std::filesystem;


class DirList {
public:
	DirList(std::vector<std::string> directory_list,std::vector<std::string> exclude_directory,	std::vector<std::string> mask_include,std::vector<std::string> mask_exclude, unsigned int min_size, bool level);
	~DirList() = default;


	//std::vector<std::string> getDirs();
	std::list<fs::path> getDirs();

	
private:
	std::vector<std::string> directory_list_;
	std::vector<std::string> exclude_directory_;
	std::vector<std::string> mask_include_;
	std::vector<std::string> mask_exclude_;

	bool level_;
	unsigned int min_size_;
	std::list<fs::path> result_directories_;

};


