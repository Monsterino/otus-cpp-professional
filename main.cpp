#include <iostream>
#include <string>
#include <cstring>

#include "async.h"


int main() {



	std::string data = "";

	data += "a\n{\nb\nc\nd\n}\n{\n";
	for (size_t i = 0; i < 100000; i++)
	{ 
		data += std::to_string(i);
		data += "\n";
	}

	data += "}\na\nq\nw\ne\nk\n";
	
	char* char_array = new char[data.length() + 1];
	strcpy(char_array, data.c_str());

	
	auto ct = async::connect(5);
	
	async::receive(ct,char_array, data.length());
	async::receive(ct, "z\nx\nc\nv\nb\nm\n", 12);
	async::disconnect(ct);

	return 0;
};

