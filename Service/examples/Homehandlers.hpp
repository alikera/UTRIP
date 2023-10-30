#ifndef HOME_HPP
#define HOME_HPP

#include "../server/server.hpp"
#include "Utrip.hpp"

class ApplyFilter : public RequestHandler
{
public:
	ApplyFilter(Utrip* utr) : utripservice(utr) {}
	Response *callback(Request *);
private:
	Utrip* utripservice;
};

class HotelInfo : public RequestHandler
{
public:
	HotelInfo(Utrip* utr) : utripservice(utr) {}
	Response *callback(Request *);
private:
	Utrip* utripservice;
	
};
class Home : public RequestHandler
{
public:
	Home(Utrip* utr) : utripservice(utr) {}
	Response *callback(Request *);
private:
	Utrip* utripservice;	
};

#endif