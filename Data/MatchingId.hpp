#ifndef MATCHING_ID
#define MATCHING_ID

#include <string>
#include "Hotel.hpp"

class MatchingId
{
public:
	MatchingId(std::string id_);
	bool operator()(Hotel* hotel);
private:
	std::string id;	
};

#endif