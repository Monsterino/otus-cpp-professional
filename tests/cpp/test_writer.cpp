#include <gtest/gtest.h>

#include "async.h"
#include "utils.h"
#include "barrier.h"

#include <random>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>


#include <filesystem>


namespace fs = std::filesystem;


TEST(Writer_test, One_writer_static_output) {
	//????????? ???????????? ?????? ? ???? ?????? ? ?????? ?????? ?????? Writer, ?????? ???????????? ??????? (5 ?????? ?? ?????)
	std::string test_time = "Test_time";
	int bulk_size = 5;
	auto test_writer_ = std::shared_ptr<Writer>(new Writer(1));


	std::vector<std::vector<std::string>> result_bulk_massive;          // ??????????????? ?????? ??????? ?????? ???????
	std::vector<std::string> generated_commands;
	//?????????? ?????? ??? ?????
	for (size_t i = 0; i < 10000; i++)						// ?????????? ?????? 
	{
		if (!(i % bulk_size) && i != 0)
		{
			std::sort(generated_commands.begin(), generated_commands.end());
			result_bulk_massive.push_back(generated_commands);
			generated_commands.clear();
		}
		generated_commands.push_back(std::to_string(i));
	}

	result_bulk_massive.push_back(generated_commands);
	generated_commands.clear();

	// ???????? ????? writer ??? ??????? ??????
	for (auto bulk : result_bulk_massive)
	{
		test_writer_->write(bulk, test_time);
	}

	test_writer_.reset();
	
	auto current_path = fs::current_path();

	//???????? ?? ??????????, ??????? ????? ????, ? ??????? ???????, ??? ???? - ??? ????? ??????, ? ???????? - ?????, ??????? ? ???? ?????????
	std::map<int, std::vector<std::string>> paths;

	for (auto dir : fs::directory_iterator(current_path))
	{
		std::string dr = dir.path().generic_string();
		std::string filename = dir.path().filename().generic_string();;
		std::vector<std::string> koeffs;
		if (dr.find("Context_id_") != std::string::npos) {
			replacer(filename, "Context_id_", "");
			replacer(filename, "bulk_", "");
			replacer(filename, "file_", "");
			koeffs = splitter(filename, '_');
			int bulk_num = std::stoi(koeffs[1]);
			if (paths.count(bulk_num))
			{
				std::vector<std::string> bulk_paths = paths[bulk_num];
				bulk_paths.push_back(dr);
				paths[bulk_num] = bulk_paths;
			}
			else
			{
				paths[bulk_num] = std::vector<std::string>{ dr };
			}
		}

	}

	//????????? ?????? ????, ?????? ??????, ????????? ?????????????? ???? ???????, ??????? ????????? ??? ?????
	std::vector<std::vector<std::string>> result_data;
	std::vector<std::string> log_files;
	for (const auto& [key, value] : paths)
	{
		std::vector<std::string> vec_of_file;
		for (auto path : value)
		{
			std::ifstream inputFile;

			inputFile.open(path);

			if (!inputFile.is_open())
			{
				std::cout << "Failed to open file" << std::endl;
			}

			std::string line;
			std::istringstream is;

			while (!inputFile.eof())
			{
				std::getline(inputFile, line);

				replacer(line, "bulk: ", "");
				vec_of_file = splitter(line, ' ');

			}
			log_files.insert(log_files.end(), vec_of_file.begin(), vec_of_file.end());
			inputFile.close();
			fs::remove(path);
		}
		std::sort(log_files.begin(), log_files.end());
		result_data.push_back(log_files);
		log_files.clear();
	}
	

	ASSERT_EQ(result_bulk_massive.size(), result_data.size());

	for (int i = 0; i < result_bulk_massive.size(); i++)
	{
		EXPECT_EQ(result_bulk_massive[i], result_data[i]);
	}

}

TEST(Writer_test, One_writer_dynamic_output) {
	//????????? ???????????? ?????? ? ???? ?????? ? ?????? ?????? ?????? Writer, ?????? ????????????? ??????? (2-8 ?????? ?? ?????)
	std::string test_time = "Test_time";
	auto test_writer_ = std::unique_ptr<Writer>(new Writer(1));


	std::vector<std::vector<std::string>> result_bulk_massive;          // ??????????????? ?????? ??????? ?????? ???????
	std::vector<std::string> generated_commands;
	//?????????? ?????? ??? ?????

	std::mt19937 generator;
	std::uniform_int_distribution<int> distribution(1, 8);				// ????????? ?????????? ????? ?? 1 ?? 8
	int bulk_size = distribution(generator);
	for (size_t i = 0; i < 10000; i++)						// ?????????? ?????? 
	{
		if (!(i % bulk_size) && i != 0)
		{
			std::sort(generated_commands.begin(), generated_commands.end());
			result_bulk_massive.push_back(generated_commands);
			generated_commands.clear();
			bulk_size = distribution(generator);
		}
		generated_commands.push_back(std::to_string(i));
	}

	result_bulk_massive.push_back(generated_commands);
	generated_commands.clear();

	// ???????? ????? writer ??? ??????? ??????

	for (auto bulk : result_bulk_massive)
	{
		test_writer_->write(bulk, test_time);
	}

	test_writer_.reset();

	auto current_path = fs::current_path();

	//???????? ?? ??????????, ??????? ????? ????, ? ??????? ???????, ??? ???? - ??? ????? ??????, ? ???????? - ?????, ??????? ? ???? ?????????

	std::map<int, std::vector<std::string>> paths;
	for (auto dir : fs::directory_iterator(current_path))
	{
		std::string dr = dir.path().generic_string();
		std::string filename = dir.path().filename().generic_string();;

		std::vector<std::string> koeffs;
		if (dr.find("Context_id_") != std::string::npos) {
			replacer(filename, "Context_id_", "");
			replacer(filename, "bulk_", "");
			replacer(filename, "file_", "");
			koeffs = splitter(filename, '_');
			int bulk_num = std::stoi(koeffs[1]);
			if (paths.count(bulk_num))
			{
				std::vector<std::string> bulk_paths = paths[bulk_num];
				bulk_paths.push_back(dr);
				paths[bulk_num] = bulk_paths;
			}
			else
			{
				paths[bulk_num] = std::vector<std::string>{ dr };
			}
		}

	}

	std::vector<std::vector<std::string>> result_data;
	std::vector<std::string> log_files;
	for (const auto& [key, value] : paths)
	{
		std::vector<std::string> vec_of_file;
		for (auto path : value)
		{
			std::ifstream inputFile;

			inputFile.open(path);

			if (!inputFile.is_open())
			{
				std::cout << "Failed to open file" << std::endl;
			}

			std::string line;
			std::istringstream is;

			while (!inputFile.eof())
			{
				std::getline(inputFile, line);

				replacer(line, "bulk: ", "");
				vec_of_file = splitter(line, ' ');

			}
			log_files.insert(log_files.end(), vec_of_file.begin(), vec_of_file.end());
			inputFile.close();
			fs::remove(path);
		}
		std::sort(log_files.begin(), log_files.end());
		result_data.push_back(log_files);
		log_files.clear();
	}

	
	ASSERT_EQ(result_bulk_massive.size(), result_data.size());

	for (int i = 0; i < result_bulk_massive.size(); i++)
	{
		EXPECT_EQ(result_bulk_massive[i], result_data[i]);
	}

}

TEST(Writer_test, Two_writer_static_output) {
	//????????? ???????????? ?????? ? ???? ?????? ? ?????? ?????? ???? Writer, ?????? ???????????? ??????? (5 ?????? ?? ?????)
	std::string test_time = "Test_time";
	int bulk_size = 5;
	auto test_writer_ = std::shared_ptr<Writer>(new Writer(1));


//	std::vector<std::vector<std::string>> result_bulk_massive;          // ??????????????? ?????? ??????? ?????? ???????
	std::vector<std::string> generated_commands;
	//?????????? ?????? ??? ?????
	std::vector<std::vector<std::string>> first_th_bulk_massive;          // ?????? ??????? ??????? ??????? ??? ??????? ???????
	std::vector<std::vector<std::string>> second_th_bulk_massive;		  // ?????? ??????? ??????? ??????? ??? ??????? ???????

	std::mt19937 generator;

	std::uniform_int_distribution<int> distribution_t(0, 1);			// C???????? ????? ???????, ??????? ????? ???????????? ????? ??????

	for (size_t i = 0; i < 10000; i++)						// ?????????? ?????? 
	{
		if (!(i % bulk_size) && i != 0)
		{
			std::sort(generated_commands.begin(), generated_commands.end());

			distribution_t(generator) ? first_th_bulk_massive.push_back(generated_commands)
				: second_th_bulk_massive.push_back(generated_commands);
			generated_commands.clear();
		}
		generated_commands.push_back(std::to_string(i));
	}


	distribution_t(generator) ? first_th_bulk_massive.push_back(generated_commands)
		: second_th_bulk_massive.push_back(generated_commands);

	generated_commands.clear();

	c_barrier start(2);


	auto file_creater_th = [&start, &test_time](std::vector<std::vector<std::string>> bulks, int context_id) {
		auto test_writer = std::shared_ptr<Writer>(new Writer(context_id));
		start.wait();
		for (auto bulk : bulks)
		{
			test_writer->write(bulk, test_time);
		}
	};

	std::thread thr1{ file_creater_th, std::ref(first_th_bulk_massive),0 };
	std::thread thr2{ file_creater_th, std::ref(second_th_bulk_massive),1 };

	thr1.join();
	thr2.join();

		
	auto current_path = fs::current_path();

	std::map<int, std::vector<std::string>> paths_first_context;
	std::map<int, std::vector<std::string>> paths_second_context;
	std::vector<std::map<int, std::vector<std::string>>> maps{ paths_first_context ,paths_second_context };

	for (auto dir : fs::directory_iterator(current_path))
	{
		std::string dr = dir.path().generic_string();
		std::string filename = dir.path().filename().generic_string();;

		
		std::vector<std::string> koeffs;
		if (dr.find("Context_id_") != std::string::npos) {
			replacer(filename, "Context_id_", "");
			replacer(filename, "bulk_", "");
			replacer(filename, "file_", "");
			koeffs = splitter(filename, '_');
			int bulk_num = std::stoi(koeffs[1]);
			int context = std::stoi(koeffs[0]);
			if (maps[context].count(bulk_num))
			{
				std::vector<std::string> bulk_paths = maps[context][bulk_num];
				bulk_paths.push_back(dr);
				maps[context][bulk_num] = bulk_paths;
			}
			else
			{
				maps[context][bulk_num] = std::vector<std::string>{ dr };
			}
		}

	}

	std::vector<std::vector<std::string>> result_data_first_context;
	std::vector<std::vector<std::string>> result_data_second_context;
//	std::vector<std::string> log_files;

	auto file_reader_th = [](std::map<int, std::vector<std::string>>& set_of_files, std::vector<std::vector<std::string>>& paths) {
		std::vector<std::string> log_files;
		for (const auto& [key, value] : set_of_files)
		{
			std::vector<std::string> vec_of_file;
			for (auto path : value)
			{
				std::ifstream inputFile;

				inputFile.open(path);

				if (!inputFile.is_open())
				{
					std::cout << "Failed to open file" << std::endl;
				}

				std::string line;
				std::istringstream is;

				while (!inputFile.eof())
				{
					std::getline(inputFile, line);

					replacer(line, "bulk: ", "");
					vec_of_file = splitter(line, ' ');

				}
				log_files.insert(log_files.end(), vec_of_file.begin(), vec_of_file.end());
				inputFile.close();
				fs::remove(path);
			}
			std::sort(log_files.begin(), log_files.end());
			paths.push_back(log_files);
			log_files.clear();
		}

	};

	std::thread reader_thr1{ file_reader_th, std::ref(maps[0]),std::ref(result_data_first_context) };
	std::thread reader_thr2{ file_reader_th, std::ref(maps[1]),std::ref(result_data_second_context) };

	reader_thr1.join();
	reader_thr2.join();
	
	ASSERT_EQ(first_th_bulk_massive.size(), result_data_first_context.size());
	ASSERT_EQ(second_th_bulk_massive.size(), result_data_second_context.size());

	for (int i = 0; i < first_th_bulk_massive.size(); i++)
	{
		EXPECT_EQ(first_th_bulk_massive[i], result_data_first_context[i]);
	}
	for (int i = 0; i < second_th_bulk_massive.size(); i++)
	{
		EXPECT_EQ(second_th_bulk_massive[i], result_data_second_context[i]);
	}

}

TEST(Writer_test, Two_writer_dynamic_output) {
	//????????? ???????????? ?????? ? ???? ?????? ? ?????? ?????? ???? Writer, ?????? ?????????????  ??????? (2-8 ?????? ?? ?????)
	std::string test_time = "Test_time";
	
	auto test_writer_ = std::shared_ptr<Writer>(new Writer(1));


	std::vector<std::string> generated_commands;
	//?????????? ?????? ??? ?????
	std::vector<std::vector<std::string>> first_th_bulk_massive;          // ?????? ??????? ??????? ??????? ??? ??????? ???????
	std::vector<std::vector<std::string>> second_th_bulk_massive;		  // ?????? ??????? ??????? ??????? ??? ??????? ???????

	std::mt19937 generator;

	std::uniform_int_distribution<int> distribution_t(0, 1);			// C???????? ????? ???????, ??????? ????? ???????????? ????? ??????
	std::uniform_int_distribution<int> distribution(1, 8);				// ????????? ?????????? ????? ?? 1 ?? 8
	int bulk_size = distribution(generator);
	for (size_t i = 0; i < 10000; i++)						// ?????????? ?????? 
	{
		if (!(i % bulk_size) && i != 0)
		{
			std::sort(generated_commands.begin(), generated_commands.end());

			distribution_t(generator) ? first_th_bulk_massive.push_back(generated_commands)
				: second_th_bulk_massive.push_back(generated_commands);
			generated_commands.clear();
		}
		generated_commands.push_back(std::to_string(i));
		bulk_size = distribution(generator);
	}


	distribution_t(generator) ? first_th_bulk_massive.push_back(generated_commands)
		: second_th_bulk_massive.push_back(generated_commands);

	generated_commands.clear();

	c_barrier start(2);


	auto file_creater_th = [&start, &test_time](std::vector<std::vector<std::string>>& bulks, int context_id) {
		auto test_writer = std::shared_ptr<Writer>(new Writer(context_id));
		start.wait();
		for (auto bulk : bulks)
		{
			test_writer->write(bulk, test_time);
		}
	};

	std::thread thr1{ file_creater_th, std::ref(first_th_bulk_massive),0 };
	std::thread thr2{ file_creater_th, std::ref(second_th_bulk_massive),1 };

	thr1.join();
	thr2.join();


	auto current_path = fs::current_path();

	std::map<int, std::vector<std::string>> paths_first_context;
	std::map<int, std::vector<std::string>> paths_second_context;
	std::vector<std::map<int, std::vector<std::string>>> maps{ paths_first_context ,paths_second_context };

	for (auto dir : fs::directory_iterator(current_path))
	{
		std::string dr = dir.path().generic_string();
		std::string filename = dir.path().filename().generic_string();;


		std::vector<std::string> koeffs;
		if (dr.find("Context_id_") != std::string::npos) {
			replacer(filename, "Context_id_", "");
			replacer(filename, "bulk_", "");
			replacer(filename, "file_", "");
			koeffs = splitter(filename, '_');
			int bulk_num = std::stoi(koeffs[1]);
			int context = std::stoi(koeffs[0]);
			if (maps[context].count(bulk_num))
			{
				std::vector<std::string> bulk_paths = maps[context][bulk_num];
				bulk_paths.push_back(dr);
				maps[context][bulk_num] = bulk_paths;
			}
			else
			{
				maps[context][bulk_num] = std::vector<std::string>{ dr };
			}
		}

	}

	std::vector<std::vector<std::string>> result_data_first_context;
	std::vector<std::vector<std::string>> result_data_second_context;
	//	std::vector<std::string> log_files;

	auto file_reader_th = [](std::map<int, std::vector<std::string>>& set_of_files, std::vector<std::vector<std::string>>& paths) {
		std::vector<std::string> log_files;
		for (const auto& [key, value] : set_of_files)
		{
			std::vector<std::string> vec_of_file;
			for (auto path : value)
			{
				std::ifstream inputFile;

				inputFile.open(path);

				if (!inputFile.is_open())
				{
					std::cout << "Failed to open file" << std::endl;
				}

				std::string line;
				std::istringstream is;

				while (!inputFile.eof())
				{
					std::getline(inputFile, line);

					replacer(line, "bulk: ", "");
					vec_of_file = splitter(line, ' ');

				}
				log_files.insert(log_files.end(), vec_of_file.begin(), vec_of_file.end());
				inputFile.close();
				fs::remove(path);
			}
			std::sort(log_files.begin(), log_files.end());
			paths.push_back(log_files);
			log_files.clear();
		}

	};

	std::thread reader_thr1{ file_reader_th, std::ref(maps[0]),std::ref(result_data_first_context) };
	std::thread reader_thr2{ file_reader_th, std::ref(maps[1]),std::ref(result_data_second_context) };

	reader_thr1.join();
	reader_thr2.join();

	ASSERT_EQ(first_th_bulk_massive.size(), result_data_first_context.size());
	ASSERT_EQ(second_th_bulk_massive.size(), result_data_second_context.size());

	for (int i = 0; i < first_th_bulk_massive.size(); i++)
	{
		EXPECT_EQ(first_th_bulk_massive[i], result_data_first_context[i]);
	}
	for (int i = 0; i < second_th_bulk_massive.size(); i++)
	{
		EXPECT_EQ(second_th_bulk_massive[i], result_data_second_context[i]);
	}

}

