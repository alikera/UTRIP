#ifndef CRITERIADEFAULT_HPP
#define CRITERIADEFAULT_HPP

#include "Criteria.hpp"
#include <string>
#include <cmath>

class CriteriaDefault : public Criteria
{
public:
	CriteriaDefault(double average_, double deviation_);
	hotels_t apply_criteria(hotels_t hotels);
	bool is_lesser_than_deviation(double average_price_hotel);
private:
	double average;
	double deviation;
};

#endif