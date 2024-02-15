#include <iostream>
#include <string>
#include <cstring>

#include "async.h"


int main() {



	std::string data = "";

//	data += "a\n{\nb\nc\nd\n}\n{\n";
	for (size_t i = 0; i < 1000; i++)
	{ 
		data += std::to_string(i);
		data += "\n";
	}

	data += "}\na\nq\nw\ne\nk\n";
	
	char* char_array = new char[data.length() + 1];
	strcpy(char_array, data.c_str());

//	std::string data_2 = "";

	//	data += "a\n{\nb\nc\nd\n}\n{\n";
//	for (size_t i = 1000000; i < 2000000; i++)
//	{
//		data_2 += std::to_string(i);
//		data_2 += "\n";
//	}

//	data += "}\na\nq\nw\ne\nk\n";

//	char* char_array_2 = new char[data_2.length() + 1];
	//strcpy(char_array_2, data_2.c_str());

	auto ct = async::connect(5);

//	auto ct_2 = async::connect(15);

	
	async::receive(ct,char_array, data.length());
	async::receive(ct, "z\nx\nc\nv\nb\nm\n", 12);
//	async::receive(ct_2,char_array_2, data.length());
//	async::receive(ct_2, "z\nx\nc\nv\nb\nm\n", 12);
	async::disconnect(ct);
//	async::disconnect(ct_2);

	return 0;
};

