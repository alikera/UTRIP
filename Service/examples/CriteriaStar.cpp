#include "CriteriaStar.hpp"

using namespace std;
CriteriaStar::CriteriaStar(int min_star_, int max_star_)
{
	min_star = min_star_;
	max_star = max_star_;
}

hotels_t CriteriaStar::apply_criteria(hotels_t hotels)
{
	hotels_t filtered_hotels;
	for(auto it : hotels)
		if(it->get_stars() >= min_star && it->get_stars()<= max_star)
			filtered_hotels.push_back(it);

	return filtered_hotels;
}