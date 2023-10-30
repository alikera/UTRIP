#ifndef CrITERIAAVAILABLEROOMS_HPP
#define CrITERIAAVAILABLEROOMS_HPP

#include "Criteria.hpp"
#include "ReservationInputs.hpp"
#include <string>

class CriteriaAvailableRooms : public Criteria
{
public:
	CriteriaAvailableRooms(ReservationInputs filter_);
	hotels_t apply_criteria(hotels_t hotels);
	void criteria_on_each_hotel(hotels_t& filtered_hotels, Hotel* curr_hotel);
private:
	ReservationInputs filter;
};

#endif