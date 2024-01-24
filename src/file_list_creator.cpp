#include "file_list_creator.h"


#include <iostream>
#include <list>
#include <regex>


DirList::DirList(std::vector<std::string> directory_list, std::vector<std::string> exclude_directory, std::vector<std::string> mask_include, std::vector<std::string> mask_exclude, unsigned int min_size, bool level) :
	directory_list_(directory_list), exclude_directory_(exclude_directory), mask_include_(mask_include), mask_exclude_(mask_exclude), min_size_(min_size), level_(level)
{

	if (level_)
	{
		for (auto dirs : directory_list_) {
			fs::path directory_path = fs::path(dirs);
			for (const auto& file : fs::recursive_directory_iterator(directory_path)) {
				if (file.is_directory()) {continue;}
				result_directories_.push_back(file);
			}
		}
	}
	else
	{
		for (auto dirs : directory_list_) {
			fs::path directory_path = fs::path(dirs);
			for (const auto& file : fs::directory_iterator(directory_path)) {
				if (file.is_directory()) { continue; }
				result_directories_.push_back(file);
			}
		}
	}

	result_directories_.unique();

	for (auto dirs : exclude_directory_) {
		fs::path exclude_path = fs::path(dirs);
		for (const auto& file : fs::recursive_directory_iterator(exclude_path)) {
			result_directories_.remove(file);
		}
	}


	if (mask_include_.size())
	{
		for (auto reg : mask_include_) {
			std::regex regex(reg);


			result_directories_.erase(std::remove_if(result_directories_.begin(), result_directories_.end(), [=]
			(fs::path& path) -> bool
				{
					return !std::regex_search(path.filename().string(), regex);

				}), result_directories_.end());
		}

	}

	if (mask_exclude_.size())
	{
		for (auto reg : mask_exclude_) {
			std::regex regex(reg);


			result_directories_.erase(std::remove_if(result_directories_.begin(), result_directories_.end(), [=]
			(fs::path& path) -> bool
				{
					return std::regex_search(path.filename().string(), regex);

				}), result_directories_.end());
		}

	}

	result_directories_.erase(std::remove_if(result_directories_.begin(), result_directories_.end(), [=]
	(fs::path& path) -> bool
		{
			return fs::file_size(path) < min_size_;

		}), result_directories_.end());
}

std::list<fs::path> DirList::getDirs()
{
	return result_directories_;
};