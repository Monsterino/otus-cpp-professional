#include <iostream>
#include <cmath>
#include <string>
#include <list>
#include <tuple>
#include <vector>
#include <utility>
#include <type_traits>



template<typename Num>
typename std::enable_if<std::is_integral<Num>::value,void>::type
print_ip(const Num& number) {

	short size = sizeof(Num);
	auto element = static_cast<const unsigned char*>(static_cast<const void*>(&number));
	for (int i = size - 1; i > 0; i--)
	{
		std::cout << (int)element[i] << ".";
	}
	std::cout << (int)element[0] << std::endl;
}


template<typename Str>
typename std::enable_if<std::is_same<std::string,Str>::value,void>::type
print_ip(const Str& string_text) {
	std::cout << string_text << std::endl;
}

template<typename Vec>
decltype(std::begin(std::declval<Vec>()), std::end(std::declval<Vec>()),typename std::enable_if<!std::is_same<std::string,Vec>::value,void>::type())

print_ip(const Vec& vector) {
	auto current_element = std::begin(vector);
	//while (current_element!=end)
	for (size_t i = 0; i < vector.size()-1; i++)
	{
		std::cout << *current_element << ".";
		current_element = std::next(current_element);
	}
	std::cout << *current_element << std::endl;
}


//Другая реализация вывода данных из tuple, здесь отсутствует проверка на одинкаовость типов
/*
template<class Tupl, size_t... Ind>
void print_tuple_elements(const Tupl& tp, std::index_sequence<Ind...> seq) {
	(..., (std::cout << (Ind == 0? "" : ".") << std::get<Ind>(tp)));
	std::cout << std::endl;
}

template<class... T>
void print(const std::tuple<T...>& tuple) {
	print_tuple_elements(tuple, std::make_index_sequence<sizeof... (T)>());
}

template<typename Tup>
decltype(std::tuple_size<Tup>::value,void())
print_ip(const Tup& tuple) {
	print(tuple);
}*/

template<typename Tupl,size_t id,typename Last, typename Curr, typename... Tail>
struct printer{};

template<typename Tupl,size_t id, typename Current>
struct printer<Tupl,id,Current,Current>
{
	static void print_element(const Tupl& tuple) {
		std::cout << std::get<id>(tuple) << std::endl;
	}
};

template<typename Tupl, size_t id, typename Curr, typename... Tail>
struct printer<Tupl,id,Curr,Curr,Tail...>
{
	static void print_element(const Tupl& tuple) {
		std::cout << std::get<id>(tuple) << ".";
		printer<Tupl, id + 1, Curr, Tail...>::print_element(tuple);
	}
};


template<typename Head, typename... Tail>
void print(const std::tuple<Head, Tail...>& tuple) {
	printer<std::tuple<Head, Tail...>, 0, Head, Head, Tail...>::print_element(tuple);
}

template<typename Tup>
decltype(std::tuple_size<Tup>::value, void())
print_ip(const Tup& tuple) {
	print(tuple);
}
int main(){
	print_ip(int8_t{ -1 }); // 255
	print_ip(int16_t{ 0 }); // 0.0
	print_ip(int32_t{ 2130706433 }); // 127.0.0.1
	print_ip(int64_t{ 8875824491850138409 }); // 123.45.67.89.101.112.131.41
	print_ip(std::string{"Hello, World!"}); // Hello, World!
	print_ip(std::vector<int>{100, 200, 300, 400}); // 100.200.300.400
	
	print_ip(std::list<short>{400, 300, 200, 100}); // 400.300.200.100 
	print_ip(std::make_tuple(123, 456, 789, 0)); // 123.456.789.0

    return 0;
}
