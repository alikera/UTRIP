#ifndef WELCOME_HPP
#define WELCOME_HPP

#include "../server/server.hpp"
#include "Utrip.hpp"

class WelcomePage : public RequestHandler
{
public:
	WelcomePage(Utrip* utr) : utripservice(utr) {}
	Response *callback(Request *);
private:
	Utrip* utripservice;
};

#endif