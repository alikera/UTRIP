#include "CriteriaAvailableRooms.hpp"
#include "exception"
using namespace std;
CriteriaAvailableRooms::CriteriaAvailableRooms(ReservationInputs filter_)
{
	filter = filter_; 
}

hotels_t CriteriaAvailableRooms::apply_criteria(hotels_t hotels)
{
	hotels_t filtered_hotels;
	for(auto it : hotels)
		criteria_on_each_hotel(filtered_hotels,it);

	return filtered_hotels;
}

void CriteriaAvailableRooms::criteria_on_each_hotel(hotels_t& filtered_hotels, Hotel* curr_hotel)
{
	try
	{
		rooms_t hotel_rooms = curr_hotel->choose_room_type(filter);
		curr_hotel->check_for_enough_rooms(hotel_rooms, filter);
		filtered_hotels.push_back(curr_hotel);
	}catch(exception& ex){}
}