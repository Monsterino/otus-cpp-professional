#include "barrier.h"

c_barrier::c_barrier(int num_threads) :num_threads_(num_threads), counter_(0), waiting_(0) {};

void c_barrier::wait() {
	std::unique_lock<std::mutex> lock(mutex_);
	counter_++;
	waiting_++;
	while (counter_ < num_threads_)
	{
		condition_.wait(lock);
	}
	condition_.notify_all();
	waiting_--;
	if (!waiting_)
	{
		counter_ = 0;
	}
	lock.unlock();
}

