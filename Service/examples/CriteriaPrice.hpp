#ifndef CrITERIAPRICE_HPP
#define CrITERIAPRICE_HPP

#include "Criteria.hpp"
#include <string>

class CriteriaPrice : public Criteria
{
public:
	CriteriaPrice(double min_price_, double max_price_);
	hotels_t apply_criteria(hotels_t hotels);
private:
	double min_price;
	double max_price;
};

#endif