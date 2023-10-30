#ifndef ORDER_HPP
#define ORDER_HPP
#include <string>
#include "Hotel.hpp"
#include <iostream>
#include "defines.hpp"

template <typename T>
class Order
{
public:
	Order(Hotel* h1, Hotel* h2, T (Hotel::*getter_)(), std::string order_);
	bool operator()();
	
private:
	Hotel* hotel_1;
	Hotel* hotel_2;
	T (Hotel::*getter)() = NULL;
	std::string order;
};

#endif