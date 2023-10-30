#include "Welcomehandler.hpp"
#include "defineshandlers.hpp"

using namespace std;

Response *WelcomePage::callback(Request *req)
{
	if(utripservice->user_is_logged_in(req->getSessionId()))
		return Response::redirect("/home");

	Response *res = new Response;
	res->setHeader("Content-Type", "text/html");
	ostringstream body;
	body
		<< "<html>" << endl
	    << "    <head><title>Utrip!</title>" << endl
	    << "<style>" << endl
		<< "li {" << endl
		<< "    display: inline;" << endl << "}" << endl
		<< "</style>" << endl
		<< "    	<ul>" << endl
	    << "		  <li><a href='signup'>Signup</a></li>" << endl
	    << "		  <li><a href='login'>Login</a></li>" << endl
	    << "        </ul>" << endl
	    << "    </head>" << endl
	    << "    <body>" << endl
	    << "		<h1> You Are Not Logged in </h1>" << endl
	    << "		<h2> Sign up or Login to use Utrip services </h2>" << endl
	    << "    </body>" << endl
	    << "</html>" << endl;

	res->setBody(body.str());
	return res;
}