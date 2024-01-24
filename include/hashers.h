#pragma once
#include <vector>
#include <string>
#include <filesystem>


//namespace fs = std::filesystem;



struct Hasher
{
	virtual ~Hasher() = default;
	virtual std::size_t getHash(void* data, std::size_t length) { return 0; };
};


struct Crc32 : public Hasher
{
public:
	Crc32() {};
	~Crc32() = default;
	std::size_t getHash(void* data, std::size_t length) override;

};


struct Crc16 : public Hasher
{
public:
	Crc16() {};
	~Crc16() = default;
	std::size_t getHash(void* data, std::size_t length) override;

};
