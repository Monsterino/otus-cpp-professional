#pragma once
#include <vector>

class filter {
	public:
		filter();
		void sort(bool reverse = false);
		void sort(std::vector<long long>& container,bool reverse = false);

		void filter_any(const int& byte);
		void search(const short& first = -1,const short& second = -1,const short& third = -1,const short& fourth = -1);
		void get_ips(std::vector<long long> vec);

		void show_ips();
		void show_ips(std::vector<long long> vec);


		void add_ip(long long ip);

	private:

		std::vector<long long> search_bytes(const std::vector<long long>& array ,const short& search_value, const int& byte);
		void get_ip(const long long& ip, std::vector<int>& vec);
		std::vector<long long> ip_container;
		//long long* sorted_container;
		bool _reverse;




};

