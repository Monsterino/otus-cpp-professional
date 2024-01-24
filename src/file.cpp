#include "file.h"

File::File(fs::path path):path_(path) {
	file_stream.open(path);
};

File::~File() {
	close_stream();
}


std::ifstream& File::get_stream() {	return file_stream;};
fs::path File::get_path() {	return path_;};
void File::close_stream() {
	if (file_stream.is_open())
	{
		file_stream.close();
	}
};