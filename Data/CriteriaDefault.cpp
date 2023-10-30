#include "CriteriaDefault.hpp"
#include <algorithm>
using namespace std;

CriteriaDefault::CriteriaDefault(double average_, double deviation_)
{
	average = average_;
	deviation = deviation_;
}

hotels_t CriteriaDefault::apply_criteria(hotels_t hotels)
{
	hotels_t filtered_hotels (hotels.size());
	auto it = copy_if(hotels.begin(), hotels.end(), filtered_hotels.begin(), [this](Hotel* h1)
			{
				return abs(h1->get_average_price() - average) <= 2 * deviation;
			});
	filtered_hotels.resize(distance(filtered_hotels.begin(), it));
	
	return filtered_hotels;
}

bool CriteriaDefault::is_lesser_than_deviation(double average_price_hotel)
{
	return abs(average_price_hotel - average) <= 2 * deviation;
}