#pragma once
#include <vector>
#include <iostream>
#include <tuple>

#include "node.h"

template<typename T, size_t D,typename Head, typename... Tail>
struct tuple_typer
{
	using tuple_type = typename tuple_typer<T, D - 1, Head, Head, Tail...>::tuple_type;
};

template<typename T, typename Head, typename... Tail>
struct tuple_typer<T,2, Head,Tail...>
{
	using tuple_type = std::tuple<Head,Head,T>;
};

template<typename T,typename Head>
struct tuple_typer<T,1,Head>
{
	using tuple_type = std::tuple<Head, T>;
};


template<typename T,size_t D, size_t... Indicies>
auto tupleCreator(Node<T, D>* node, std::index_sequence<Indicies...>) {
	return std::make_tuple(node->indexes[Indicies]..., node->value);
}

template<typename T,size_t D>
auto return_tuple(Node<T,D>* node) {
	auto tuple = tupleCreator(node, std::make_index_sequence<D>());
	return tuple;
};
