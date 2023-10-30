#ifndef LOGOUT_HPP
#define LOGOUT_HPP value

#include "../server/server.hpp"
#include "Utrip.hpp"

class LogoutUser : public RequestHandler
{
public:
	LogoutUser(Utrip* utr) : utripservice(utr) {}
	Response *callback(Request *);
private:
	Utrip* utripservice;
	
};

#endif
