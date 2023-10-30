#include "Room.hpp"
#include "defines.hpp"
#include <iostream>
using namespace std;

void Room::set_room_id(string id)
{
	room_id = id;
}

void Room::set_price(int price_)
{
	price = price_;
}

bool Room::is_reservable(int check_in, int check_out)
{
	for(auto const& it : periods)
	{
		if(check_in > it.second)
			;
		else if(check_out < it.first)
			;
		else
			return false;
	}
	return true;
}

Room* Room::reserve(int check_in, int check_out)
{
	periods.push_back(make_pair(check_in, check_out));
	return this;
}

void Room::free_reservation_period(int check_in)
{
	for(auto it = periods.begin(); it != periods.end(); ++it)
		if(it->first == check_in)
			it = periods.erase(it);
}

