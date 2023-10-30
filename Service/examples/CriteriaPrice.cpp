#include "CriteriaPrice.hpp"
using namespace std;

CriteriaPrice::CriteriaPrice(double min_price_, double max_price_)
{
	min_price = min_price_;
	max_price = max_price_;
}

hotels_t CriteriaPrice::apply_criteria(hotels_t hotels)
{
	hotels_t filtered_hotels;
	for(auto it : hotels)
		if(it->get_average_price() >= min_price && it->get_average_price()<= max_price)
			filtered_hotels.push_back(it);

	return filtered_hotels;
}