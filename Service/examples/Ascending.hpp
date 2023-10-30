#ifndef ASCENDING_HPP
#define ASCENDING_HPP
#include <string>
#include "Hotel.hpp"

template <typename T>
class Ascending
{
public:
	Ascending(Hotel* h1, Hotel* h2, T (Hotel::*getter_)()) : hotel_1(h1), hotel_2(h2), getter(getter_) {}
	bool operator()()
	{
	if((hotel_1->*getter)() == (hotel_2->*getter)())
		return hotel_1->get_uid() < hotel_2->get_uid();

	return (hotel_1->*getter)() < (hotel_2->*getter)(); 
	};
private:
	Hotel* hotel_1;
	Hotel* hotel_2;
	T (Hotel::*getter)() = NULL;
};

#endif