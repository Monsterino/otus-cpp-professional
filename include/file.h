#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>


namespace fs = std::filesystem;


class File
{
public:
	File(fs::path path);
	~File();

	std::ifstream& get_stream();
	fs::path get_path();

	void close_stream();
private:
	std::ifstream file_stream;
	fs::path path_;
};

