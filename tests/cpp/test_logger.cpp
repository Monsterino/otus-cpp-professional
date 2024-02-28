#include <gtest/gtest.h>

#include "async.h"
#include "utils.h"
#include "barrier.h"

#include <random>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <condition_variable>

TEST(Logger_test, One_thread_static_output) {
	//��������� ������������ ������ � ������� ������ � ������ ������ ������������� ������ � ������� ������ ������������ ������� (5 ������ �� �����)
    auto logger_ = std::shared_ptr<Logger>(new Logger());
	std::vector<std::string> tasks;
	std::string test_time = "Test_time";
	std::stringstream buffer;											//������� ����� � ������������� �����
	std::streambuf* p_cout_streambuf = std::cout.rdbuf(buffer.rdbuf());

	std::string expected;
	std::vector <std::string> expected_task;

	
	for (size_t i = 0; i < 10000; i++)									//���������� ��������� �����
	{

		if (!(i % 5) && i != 0)
		{
			expected += text_configurator(expected_task);
			expected_task.clear();
		}
		expected_task.push_back(std::to_string(i));

	}
	expected += text_configurator(expected_task);
	expected_task.clear();
	
	for (size_t i = 0; i < 10000; i++)
	{
		if (!(i%5)&&i!=0)
		{
			logger_->write(tasks, test_time);
			tasks.clear();
		}
		tasks.push_back(std::to_string(i));

	}
	logger_->write(tasks, test_time);
	tasks.clear();

	logger_.reset();														//�������� ���������� �������
	
	std::string text = buffer.str();										//���������� ����� � string 

	std::cout.rdbuf(p_cout_streambuf);										//���������� �������������� �����

    ASSERT_EQ(text, expected);
}

TEST(Logger_test, Two_thread_static_output) {
	//��������� ������������ ������ � ������� ������ � ������ ������ ���� �������� ������ � ������� ������ ������������ ������� (5)
	std::stringstream buffer;											//������� ����� � ������������� �����
	std::streambuf* p_cout_streambuf = std::cout.rdbuf(buffer.rdbuf());

	std::vector<std::vector<std::string>> first_th_bulk_massive;          // ������ ������� ������� ������� ��� ������� �������
	std::vector<std::vector<std::string>> second_th_bulk_massive;		  // ������ ������� ������� ������� ��� ������� �������


	std::mt19937 generator;
	
	std::uniform_int_distribution<int> distribution_t(0, 1);			// C�������� ����� �������, ������� ����� ������������ ����� ������

	int bulk_size =5;													// ������ ����������� ������ �����

	// ���� ��������� ������� ������� ������ � ������� ���������
	std::vector<std::string> generated_commands;
	for (size_t i = 0; i < 10000; i++)
	{
		if (!(i % bulk_size) && i != 0)									
		{
			(distribution_t(generator) ? first_th_bulk_massive.push_back(generated_commands)
				: second_th_bulk_massive.push_back(generated_commands));		//��������� ������� ����������, ����� ������ ����� ������������ �������

			generated_commands.clear();
		}
		generated_commands.push_back(std::to_string(i));
	}

	(distribution_t(generator) ? first_th_bulk_massive.push_back(generated_commands)
		: second_th_bulk_massive.push_back(generated_commands));
	generated_commands.clear();


	//�������� ����� �� �������
	std::vector<std::string> expected_dynamic_bulks;
	std::ostringstream expected_output_stream;
	std::vector<std::vector<std::string>> result_bulk_massive(first_th_bulk_massive.begin(), first_th_bulk_massive.end());
	result_bulk_massive.insert(result_bulk_massive.end(), second_th_bulk_massive.begin(), second_th_bulk_massive.end());

	for (auto bulk : result_bulk_massive)
	{
		std::string sep = "";
		expected_output_stream << "bulk: ";
		for (auto command : bulk)
		{
			expected_output_stream << sep << command;
			sep = ", ";
		}
		expected_dynamic_bulks.push_back(expected_output_stream.str());
		expected_output_stream.str("");
	}


	//�������� ������ �� �������
	c_barrier start(2);
	std::string test_time = "Test_time";			//������ �������� ��� �������

	auto worker = [&start, &test_time](std::vector<std::vector<std::string>>& bulks) {
		auto logger_first = std::shared_ptr<Logger>(new Logger());
		start.wait();
		for (auto bulk : bulks)
		{
			logger_first->write(bulk, test_time);
		}
	};

	std::thread thr1{ worker, std::ref(first_th_bulk_massive) };
	std::thread thr2{ worker, std::ref(second_th_bulk_massive) };

	thr1.join();
	thr2.join();

	std::string text = buffer.str();
	std::cout.rdbuf(p_cout_streambuf);
	auto vec = splitter(text, '\n');

	std::sort(vec.begin(), vec.end());
	std::sort(expected_dynamic_bulks.begin(), expected_dynamic_bulks.end());

	ASSERT_EQ(vec.size(), expected_dynamic_bulks.size());

	for (int i = 0; i < vec.size(); i++)
	{
		EXPECT_EQ(vec[i], expected_dynamic_bulks[i]);
	}
}

TEST(Logger_test, One_thread_dynamic_output) {
	//��������� ������������ ������ � ������� ������ � ������ ������ ������������� ������ � ������� ������� ������ �������� ( 2-8 ������)
	std::stringstream buffer;											//������� ����� � ������������� �����
	std::streambuf* p_cout_streambuf = std::cout.rdbuf(buffer.rdbuf());

	auto logger_ = std::shared_ptr<Logger>(new Logger());				//������� ����� ������
	std::string test_time = "Test_time";								//������ �������� ��� �������
	

	std::string expected;												// ��������� ��������� ��������, ������� �� ����� ������� �� �������
	
	std::vector<std::vector<std::string>> result_bulk_massive;          // ��������������� ������ ������� ������� �������
	std::vector<std::string> generated_commands;

	std::mt19937 generator;												// ��������� ���������� ����� �� 1 �� 8
	std::uniform_int_distribution<int> distribution(1, 8);
	

	int bulk_size = distribution(generator);							// ���������� ������ ��������� �������� ������� ������

	// ���� ��������� ������� ������� ������ � ������� ���������
	for (size_t i = 0; i < 10000; i++)						// ���������� ������ � ������ ��������� ������, �� 1 �� 8
	{
		if (!(i % bulk_size) && i != 0)
		{
			result_bulk_massive.push_back(generated_commands);
			generated_commands.clear();
			bulk_size = distribution(generator);
		}
		generated_commands.push_back(std::to_string(i));
	}

	result_bulk_massive.push_back(generated_commands);
	generated_commands.clear();

	
	
	//��������� ������ � ������ ������ ������ �� �������

	std::ostringstream expected_output_stream;
	for (auto bulk: result_bulk_massive)
	{
		std::string sep = "";
		expected_output_stream << "bulk: ";
		for (auto command: bulk)
		{
			expected_output_stream << sep << command;
			sep = ", ";
		}
		expected_output_stream << std::endl;
	}

	expected = expected_output_stream.str();
	expected_output_stream.clear();


	// ���������� ��� ������ ������� ������� � ����� � ������������� ����� ������, ����������� � �������
	for (auto bulk : result_bulk_massive)
	{
		logger_->write(bulk, test_time);
	}


	logger_.reset();														//�������� ���������� �������

	std::string text = buffer.str();										//���������� ����� � string 
	std::cout.rdbuf(p_cout_streambuf);										//���������� �������������� �����

	ASSERT_EQ(text, expected);
}

TEST(Logger_test, Two_thread_dynamic_output) {
	//��������� ������������ ������ � ������� ������ � ������ ������ ���� �������� ������ � ������� ������ ������������� ������� (�� 1 �� 8)
	std::stringstream buffer;											//������� ����� � ������������� �����
	std::streambuf* p_cout_streambuf = std::cout.rdbuf(buffer.rdbuf());
			



	std::vector<std::vector<std::string>> first_th_bulk_massive;          // ������ ������� ������� ������� ��� ������� �������
	std::vector<std::vector<std::string>> second_th_bulk_massive;		  // ������ ������� ������� ������� ��� ������� �������
	

	std::mt19937 generator;												
	std::uniform_int_distribution<int> distribution(1, 8);				// ��������� ���������� ����� �� 1 �� 8
	std::uniform_int_distribution<int> distribution_t(0, 1);			// C�������� ����� �������, ������� ����� ������������ ����� ������

	int bulk_size = distribution(generator);							// ���������� ������ ��������� �������� ������� �a����

	// ���� ��������� ������� ������� ������ � ������� ���������
	std::vector<std::string> generated_commands;
	for (size_t i = 0; i < 10000; i++)						
	{
		if (!(i % bulk_size) && i != 0)									// ���������� ������ � ������ ��������� ������, �� 1 �� 8
		{
			(distribution_t(generator) ? first_th_bulk_massive.push_back(generated_commands)
									   : second_th_bulk_massive.push_back(generated_commands));		//��������� ������� ����������, ����� ������ ����� ������������ �������
		
			generated_commands.clear();
			bulk_size = distribution(generator);
		}
		generated_commands.push_back(std::to_string(i));
	}

	(distribution_t(generator) ? first_th_bulk_massive.push_back(generated_commands)
							   : second_th_bulk_massive.push_back(generated_commands));
	generated_commands.clear();


	//�������� ����� �� �������
	std::vector<std::string> expected_dynamic_bulks;
	std::ostringstream expected_output_stream;
	std::vector<std::vector<std::string>> result_bulk_massive(first_th_bulk_massive.begin(), first_th_bulk_massive.end());
	result_bulk_massive.insert(result_bulk_massive.end(), second_th_bulk_massive.begin(), second_th_bulk_massive.end());

	for (auto bulk : result_bulk_massive)
	{
		std::string sep = "";
		expected_output_stream << "bulk: ";
		for (auto command : bulk)
		{
			expected_output_stream << sep << command;
			sep = ", ";
		}
		expected_dynamic_bulks.push_back(expected_output_stream.str());
		expected_output_stream.str("");
	}


	//�������� ������ �� �������
	c_barrier start(2);
	std::string test_time = "Test_time";			//������ �������� ��� �������

	auto worker = [&start, &test_time](std::vector<std::vector<std::string>>& bulks) {
		auto logger_first = std::shared_ptr<Logger>(new Logger());
		start.wait();
		for (auto bulk : bulks)
		{
			logger_first->write(bulk, test_time);
		}
	};

	std::thread thr1{ worker, std::ref(first_th_bulk_massive) };
	std::thread thr2{ worker, std::ref(second_th_bulk_massive) };
	
	thr1.join();
	thr2.join();

	std::string text = buffer.str();
	std::cout.rdbuf(p_cout_streambuf);
	auto vec = splitter(text, '\n');
	
	std::sort(vec.begin(), vec.end());
	std::sort(expected_dynamic_bulks.begin(), expected_dynamic_bulks.end());

	ASSERT_EQ(vec.size(), expected_dynamic_bulks.size());

	for (int i = 0; i < vec.size(); i++)
	{
		EXPECT_EQ(vec[i], expected_dynamic_bulks[i]);
	}

}

