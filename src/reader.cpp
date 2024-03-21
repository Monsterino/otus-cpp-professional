
#include "reader.h"


FileReader::FileReader(std::string& path,uintmax_t start, uintmax_t lenght):start_(start),lenght_(lenght),path_(path),readed_bytes_(0),end_stream_(false){
    input_stream_.open(path_);
    input_stream_.seekg(start,input_stream_.beg);
}


bool FileReader::check_state() {
    if (!input_stream_.is_open() || end_stream_ || readed_bytes_>=lenght_)
        return false;
    else {
        return true;
    }
}

std::string FileReader::get_data() {
    std::string line;
    std::getline(input_stream_,line);
    readed_bytes_+= line.size()+1;
    if (readed_bytes_>=lenght_) {
        end_stream_ = true;
    }
    return line;
}