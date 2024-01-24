#include "dublicate_finder.h"

#include <sstream>
#include <set>


DFinder::DFinder(std::list<fs::path> files, int size_block, std::string hash) : size_block_(size_block)
{
	if (hash == "crc32")
	{
		hasher.reset(new Crc32());
	}
	else
	{
		hasher.reset(new Crc16());
	}

	for (auto file : files)
	{
		std::string file_size = static_cast<std::stringstream>(std::stringstream() << fs::file_size(file)).str();
		std::size_t hash_size = hasher->getHash(file_size.data(), file_size.length());

		
		map_adder(hash_size, std::shared_ptr<File>(new File(file)), map_files_);

	}

	map_cleaner(map_files_);
	   
}


std::string DFinder::get_block(std::ifstream& stream) {
	std::string block(size_block_, '\0');
	stream.read(block.data(), size_block_);
	return block;
}

void  DFinder::check_dublicates() {
	std::unordered_map<std::size_t, std::list<std::shared_ptr<File>>> finished_file;
	while (true)
	{
		std::unordered_map<std::size_t, std::list<std::shared_ptr<File>>> tmp_files;


		for (auto it = map_files_.begin(); it != map_files_.end(); it++) 
		{
			for (std::shared_ptr<File> element : it->second)
			{
			
				if (!element->get_stream().eof())
				{
					std::string block = get_block(element->get_stream());
					std::size_t hash = hasher->getHash(block.data(), size_block_);
					map_adder(hash, element, tmp_files);
				}
				else
				{
					element->close_stream();
					map_adder(it->first, element, finished_file);

				}

			}
		}
		map_cleaner(tmp_files);
		map_files_ = tmp_files;
		if (!map_files_.size())
		{
			map_files_ = finished_file;
			break;
		}
	}
}

void DFinder::map_cleaner(std::unordered_map<std::size_t, std::list<std::shared_ptr<File>>>& map_files)
{
	for (std::unordered_map<std::size_t, std::list<std::shared_ptr<File>>>::const_iterator it = map_files.begin(); it != map_files.end();)
	{
		if (it->second.size() == 1)
		{
			it = map_files.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void DFinder::map_adder(std::size_t key, std::shared_ptr<File> value, std::unordered_map<std::size_t, std::list<std::shared_ptr<File>>>& map) {
	if (map.count(key))
	{
		auto same_files = map[key];
		same_files.push_back(value);
		map[key] = same_files;
	}
	else
	{
		map[key] = std::list<std::shared_ptr<File>>{ value };
	}
}

void DFinder::show_dublicates() {
	if (map_files_.empty()) { check_dublicates(); };

	std::cout << "========================================" << std::endl;
	for (auto element : map_files_)
	{
		for (auto file : element.second)
		{
			std::cout << file->get_path() << std::endl;
			file->close_stream();
		}
		std::cout << "========================================" << std::endl;
	}
}

std::unordered_map<std::size_t, std::list<std::shared_ptr<File>>> DFinder::get_dublicates() {
	if (map_files_.empty()) {check_dublicates();};

	return map_files_;

}