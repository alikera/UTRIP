#ifndef CrITERIA_HPP
#define CrITERIA_HPP
#include "Hotel.hpp"
#include <vector>
#include <list>
class Criteria
{
public:
	virtual hotels_t apply_criteria(hotels_t hotels) = 0;
};

typedef std::list<Criteria*> criterias_t;
#endif