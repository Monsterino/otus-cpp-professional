#pragma once
#include <condition_variable>
#include <atomic>
#include <vector>
#include <fstream>
#include <chrono>
#include <string>
#include <queue>
#include <thread>
		

#include "handler.h"




class Writer : public Handler
{
public:
	Writer(int context_id);
	~Writer();

	void write_commands(std::string name);
	void write(std::vector<std::string> &vec,std::string& time) override;
	void stop();

private:
	int id_context_;
	std::condition_variable condition_;											//��ndition variable ��� ������������� ���� �������
	std::atomic<bool> stop_;													//����������, ������� ������� � ���, ��� ���������� ��������� ������
	bool break_;																//����������, ������� �������� � ���, ��� ��� ����� �� ������� ���������
	std::mutex mutex_;								
	std::vector<std::thread> th_writers_;										//������ ���� �������, ������������ � ������ �����
	std::vector<std::string> block_;											//�������������� ���� ������
	int it_;																	//Id ��������, ������� � ������ ������ �������������� �������
	int bulk_counter_;															//����� �����, ������� � ������ ������ ��������������
	std::string filename_;														//����� �������� ������ ������� �������� �����
	std::queue<std::pair<std::vector<std::string>, std::string>> tasks_;		//������ ������, ������� ��������� �������� � ����
};


