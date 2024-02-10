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
	std::condition_variable condition_;											//Соndition variable для синхронизации двух потоков
	std::atomic<bool> stop_;													//Переменная, которая говорит о том, что необходимо завершить потоки
	bool break_;																//Переменная, которая сообщает о том, что все блоки из массива разобраны
	std::mutex mutex_;								
	std::vector<std::thread> th_writers_;										//Массив двух потоков, записывающих в разные файлы
	std::vector<std::string> block_;											//Обрабатываемый блок команд
	int it_;																	//Id элемента, который в данный момент обрабатывается потоком
	int bulk_counter_;															//Номер блока, который в данный момент обрабатывается
	std::string filename_;														//Время создания первой команды текущего блока
	std::queue<std::pair<std::vector<std::string>, std::string>> tasks_;		//Массив блоков, которые требуется записать в файл
};


