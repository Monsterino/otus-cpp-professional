#pragma once
#include <mutex>
#include <condition_variable>

// �������� ������� � �++17 ��� ������������ �������� ������������� ������
class c_barrier
{
public:
	c_barrier(int num_threads);
	void wait();

private:
	std::mutex mutex_;
	std::condition_variable condition_;
	int counter_;
	int waiting_;
	int num_threads_;
};