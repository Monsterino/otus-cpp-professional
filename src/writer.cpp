#include "writer.h"
#include <iostream>

//Writer представляет собой Action Object с двумя потоками.
Writer::Writer(int context_id) :stop_(false),break_(false),it_(-1), bulk_counter_(0), id_context_(context_id) {
	th_writers_.push_back(std::thread(&Writer::write_commands, this, "file_1"));
	th_writers_.push_back(std::thread(&Writer::write_commands, this, "file_2"));

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
	std::ofstream logfile;																//Файл, куда записываем команды
	int last_bulk = 1;																	//Последний пул команд, с которым работал поток

	while (!stop_ )															//Работаем в цикле
	{
		std::unique_lock<std::mutex> lock{ mutex_ };									//Захватываем мьютекс
		
		while (!stop_&& ((tasks_.empty() && (it_ == block_.size()|| it_ == -1))))	//Проверяем, не пришел ли флаг о закрытии и/или о том, что все файлы в очереди обработаны, есть ли задачи и в конце ли обрабатываемого блока стоит итератор
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
		if (it_ == block_.size() || it_ == -1)											//Если поток начал первым обрабатывать новый блок, то 
		{
			block_ = tasks_.front().first;												//Берем данный блок
			filename_ = tasks_.front().second;											//Время прихода первой команды этого блока (по условию)
			it_ = 0;																	//Зануляем наш итератор
			tasks_.pop();																//Забираем блок из списка блоков, которые надо обработать
			bulk_counter_++;															//Добавляем к счетчику обработанных блоков 1
		}
		
		cmd = block_[it_];																//Вытаскиваем из блока команду

		if (it_ != block_.size()){it_++;}



		if (logfile.is_open() && last_bulk!= bulk_counter_)								//Если файл открыт, и на данном этапе мы работаем не с блоком, которым работали прошлый цикл, то закрываем файл
		{
			logfile.close();
		}

		last_bulk = bulk_counter_;														//Обновляем информацию о блоке, с которым мы работали


		lock.unlock();
		condition_.notify_all();

		if (!logfile.is_open())															//Если открытых файлов нет, то создаем новый
		{
			std::string filename = "Context_id_" + std::to_string(id_context_) + "_" + "bulk_"+ std::to_string(bulk_counter_) +"_" +name + "_" + filename_ + std::string(".log");				//Название файла состоит из номера блока, названия потока, время получения первой команды этого блока
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
			std::string filename = "Context_id_" + std::to_string (id_context_) + "_" + "bulk_" + std::to_string(bulk_counter_) + "_" + name + "_" + filename_ + std::string(".log");				//Название файла состоит из номера блока, названия потока, время получения первой команды этого блока
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

