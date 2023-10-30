#ifndef WALLET_HPP
#define WALLET_HPP

#include "../server/server.hpp"
#include "Utrip.hpp"

// class ApplyFilter : public RequestHandler
// {
// public:
// 	ApplyFilter(Utrip* utr) : utripservice(utr) {}
// 	Response *callback(Request *);
// private:
// 	Utrip* utripservice;
	
// };

class ChargeWalletCredit : public RequestHandler
{
public:
	ChargeWalletCredit(Utrip* utr) : utripservice(utr) {}
	Response *callback(Request *);
private:
	Utrip* utripservice;	
};

class ShowWalletCredit : public RequestHandler
{
public:
	ShowWalletCredit(Utrip* utr) : utripservice(utr) {}
	Response *callback(Request *);
private:
	Utrip* utripservice;	
};

#endif