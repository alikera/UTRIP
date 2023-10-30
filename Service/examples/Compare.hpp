#ifndef COMPARE_HPP
#define COMPARE_HPP

#include "Hotel.hpp"

typedef std::string(Hotel::*func_ptr)();

class Compare
{
public:
	Compare(func_ptr getter) { hotel_getter = getter; };
	bool operator()(Hotel* h1, Hotel* h2) { return h1->hotel_getter() > h2->hotel_getter(); };
private:
	func_ptr hotel_getter;
};
#endif