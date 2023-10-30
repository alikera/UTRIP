#include "Wallethandlers.hpp"
#include "defineshandlers.hpp"

using namespace std;


Response *ChargeWalletCredit::callback(Request *req)
{
	double amount = stod(req->getBodyParam(AMOUNT));

	utripservice->charge_user_wallet_credit(amount, req->getSessionId());
	return Response::redirect("/wallet");
}

Response* ShowWalletCredit::callback(Request *req)
{
	if(!utripservice->user_is_logged_in(req->getSessionId()))
		return Response::redirect("/");

	Response *res = new Response;
	res->setHeader("Content-Type", "text/html");
	ostringstream body;

	body
		<< "<html>" << endl
	    << "<head><title>Wallet</title>" << endl
	    << "<style>" << endl
		<< "li {" << endl
		<< "    display: inline;" << endl << "}" << endl
		<< "</style>" << endl
		<< "    <ul>" << endl
	    << "		<li><a href='home'>Home</a></li>" << endl
	    << "		<li><a href='logout'>Logout</a></li>" << endl
	    << "	</ul>" << endl
	    << "</head>" << endl
	    << "    <body>" << endl
		<< " 		Current Wallet Credit: "
		<< utripservice->get_user_wallet_credit(req->getSessionId()) << endl
		<< "  		<form action='wallet' method='POST'>" << endl
	    << "        	Charge my Wallet: " << endl
	    << "    		<input type='number' name='amount' step='any' min='1'/> " << endl
	    << "    		<input type='submit' value='Charge'/>" << endl
	    << "  		</form>" << endl
		<< "    </body>" << endl
		<< "</html>" << endl;
	res->setBody(body.str());
	return res;
}