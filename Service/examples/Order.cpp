#include "Order.hpp"
#include <iostream>
#include <string>
using namespace std;

template<typename T>
Order<T>::Order(Hotel* h1, Hotel* h2, T (Hotel::*getter_)(), string order_)
{
	hotel_1 = h1, 
	hotel_2 = h2,
	getter = getter_;
	order = order_;
}

template<typename T>
bool Order<T>::operator()() 
{
	if((hotel_1->*getter)() == (hotel_2->*getter)())
		return hotel_1->get_uid() < hotel_2->get_uid();

	if(order == ASCENDING)
		return (hotel_1->*getter)() < (hotel_2->*getter)();

	else
		return (hotel_1->*getter)() > (hotel_2->*getter)(); 

}
