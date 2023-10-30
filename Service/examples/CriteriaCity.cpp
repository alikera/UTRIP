#include "CriteriaCity.hpp"
using namespace std;

CriteriaCity::CriteriaCity(string city_)
{
	city = city_;
}

hotels_t CriteriaCity::apply_criteria(hotels_t hotels)
{
	hotels_t filtered_hotels;
	for(auto it : hotels)
		if(it->get_city() == city)
			filtered_hotels.push_back(it);

	return filtered_hotels;
}