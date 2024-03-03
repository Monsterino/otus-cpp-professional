#include "writer.h"
#include <iostream>
#include <fstream>

//Writer ������������ ����� Action Object � ����� ��������.
Writer::Writer() :stop_(false),break_(false),it_(-1), bulk_counter_(0) {
	th_writers_.push_back(std::thread(&Writer::write_commands, this, "file_1"));
};

Writer::~Writer() {
	stop();
}



void Writer::stop() {
		
	condition_.notify_all();
	stop_ = true;

	for (size_t i = 0; i < th_writers_.size(); i++)
	{
		th_writers_[i].join();
	}
}

void Writer::write(std::vector<std::string>& vec, std::string& time) {
	const std::lock_guard<std::mutex> guard{ mutex_ };
	//block_ = vec;
	tasks_.push(std::make_pair(vec, time));
	break_ = false;
	if (it_ == -1){it_ = 0;}
	condition_.notify_all();
};

void Writer::write_commands(std::string name) 
{
	std::ofstream logfile;																//����, ���� ���������� �������
	int last_bulk = 1;																	//��������� ��� ������, � ������� ������� �����

	while (!stop_ )															//�������� � �����
	{
		std::unique_lock<std::mutex> lock{ mutex_ };									//����������� �������
		
		while (!stop_&& ((tasks_.empty() && (it_ == block_.size()|| it_ == -1))))	//���������, �� ������ �� ���� � �������� �/��� � ���, ��� ��� ����� � ������� ����������, ���� �� ������ � � ����� �� ��������������� ����� ����� ��������
		{
			if (logfile.is_open()){logfile.close();}
			condition_.wait(lock);
		}
		if (stop_) { 
			if ((tasks_.empty() && (it_ == block_.size() || it_ == -1)))
			{
				break_ = true;
			}
			break;
		}

		std::string cmd;
		if (it_ == block_.size() || it_ == -1)											//���� ����� ����� ������ ������������ ����� ����, �� 
		{
			block_ = tasks_.front().first;												//����� ������ ����
			filename_ = tasks_.front().second;											//����� ������� ������ ������� ����� ����� (�� �������)
			it_ = 0;																	//�������� ��� ��������
			tasks_.pop();																//�������� ���� �� ������ ������, ������� ���� ����������
			bulk_counter_++;															//��������� � �������� ������������ ������ 1
		}
		
		cmd = block_[it_];																//����������� �� ����� �������

		if (it_ != block_.size()){it_++;}



		if (logfile.is_open() && last_bulk!= bulk_counter_)								//���� ���� ������, � �� ������ ����� �� �������� �� � ������, ������� �������� ������� ����, �� ��������� ����
		{
			logfile.close();
		}

		last_bulk = bulk_counter_;														//��������� ���������� � �����, � ������� �� ��������


		lock.unlock();
		condition_.notify_all();

		if (!logfile.is_open())															//���� �������� ������ ���, �� ������� �����
		{
			std::string filename = "Bulk_"+ std::to_string(bulk_counter_) +"_" +name + "_" + filename_ + std::string(".log");				//�������� ����� ������� �� ������ �����, �������� ������, ����� ��������� ������ ������� ����� �����
			logfile.open(filename);
			logfile << "bulk: ";
		}
		logfile << cmd << " ";
	
	}

	condition_.notify_all();
	while (!break_)
	{
		std::unique_lock<std::mutex> lock{ mutex_ };									

		if (break_) { break; }

		std::string cmd;

		if (it_ == block_.size() || it_ == -1)											 
		{
			block_ = tasks_.front().first;												
			filename_ = tasks_.front().second;											
			it_ = 0;																	
			tasks_.pop();																
			bulk_counter_++;															
		}

		cmd = block_[it_];																

		if (it_ != block_.size()) { it_++; }

		if (logfile.is_open() && last_bulk != bulk_counter_)								
		{
			logfile.close();
		}

		last_bulk = bulk_counter_;														
		if ((tasks_.empty() && (it_ == block_.size() || it_ == -1)))
		{
			break_ = true;
		}

		lock.unlock();
		condition_.notify_all();

		if (!logfile.is_open())															
		{
			std::string filename ="Bulk_" + std::to_string(bulk_counter_) + "_" + name + "_" + filename_ + std::string(".log");				//�������� ����� ������� �� ������ �����, �������� ������, ����� ��������� ������ ������� ����� �����
			logfile.open(filename);
			logfile << "bulk: ";
		}
		logfile << cmd << " ";

	}

	condition_.notify_all();
	if (logfile.is_open())
	{
		logfile.close();
	}

}

