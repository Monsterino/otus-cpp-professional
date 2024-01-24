#include "hashers.h"

#include <boost/crc.hpp>


std::size_t Crc32::getHash(void* data, std::size_t length){
    boost::crc_32_type result;
    result.process_bytes(data, length);
    return result.checksum();
}


std::size_t Crc16::getHash(void* data, std::size_t length){
    boost::crc_16_type result;
    result.process_bytes(data, length);
    return result.checksum();
}
