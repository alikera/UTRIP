#ifndef LOGIN_HPP
#define LOGIN_HPP

#include "../server/server.hpp"
#include "Utrip.hpp"


class ShowLoginPage : public RequestHandler 
{
public:
	ShowLoginPage(Utrip* utr) : utripservice(utr) {}
	Response *callback(Request *);
private:
	Utrip* utripservice;
};

class LoginUser : public RequestHandler
{
public:
	LoginUser(Utrip* utr) : utripservice(utr) {}
	Response *callback(Request *);
	Response *login_is_successful(std::string session_id);
private:
	Utrip* utripservice;
	
};

#endif