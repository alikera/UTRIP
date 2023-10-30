#ifndef SIGNUP_HPP
#define SIGNUP_HPP

#include "../server/server.hpp"
#include "Utrip.hpp"
#include <iostream>

class CreateUser : public RequestHandler {
public:
	CreateUser(Utrip* utr) : utripservice(utr) {}
	Response *callback(Request *);
private:
	Utrip* utripservice;
};

class ShowSignupPage : public RequestHandler {
public:
	ShowSignupPage(Utrip* utr) : utripservice(utr) {}
	Response *callback(Request *);
private:
	Utrip* utripservice;
};


#endif
