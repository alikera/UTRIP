#ifndef CrITERIACITY_HPP
#define CrITERIACITY_HPP

#include "Criteria.hpp"
#include <string>

class CriteriaCity : public Criteria
{
public:
	CriteriaCity(std::string city_);
	hotels_t apply_criteria(hotels_t hotels);
private:
	std::string city;
};

#endif