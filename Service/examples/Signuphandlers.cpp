#include <string>
#include <sstream>
#include <iostream>
#include "Signuphandlers.hpp"
#include "defineshandlers.hpp"

using namespace std;

Response* CreateUser::callback(Request *req)
{
	string username = req->getBodyParam(USERNAME);
	string password = req->getBodyParam(PASSWORD);
	string re_password = req->getBodyParam(RE_PASSWORD);
	string email = req->getBodyParam(EMAIL);

	if(utripservice->have_empty_field(username, password, email))
		throw Server::Exception(EMPTY_FIELD);
	
	if(!utripservice->username_and_email_are_unique(username, email))
		throw Server::Exception(ALREADY_EXIST);

	if(password != re_password)
		throw Server::Exception(DOESNT_MATCH);

	utripservice->create_user(username, password, email);
	return Response::redirect("/login");
}

Response *ShowSignupPage::callback(Request *req)
{
	if(utripservice->user_is_logged_in(req->getSessionId()))
		return Response::redirect("/home");

	Response *res = new Response;
	res->setHeader("Content-Type", "text/html");

	ostringstream body;
	body
		<< "<html>" << endl
		<< "<head><title>Sign up</title></head>" << endl
		<< "<body>" << endl
		<< "  <form action='signup' method='POST'>" << endl
		<< "    Username: <input type='text' name='username'/><br/>" << endl
		<< "    Password: <input type='password' name='password'/><br/>" << endl
		<< "    Re-enter Password: <input type='password' name='re-password'/><br/>" << endl
		<< "    email: <input type='text' name='email'/><br/><br/>" << endl
		<< "    <input type='submit' value='Sign up'/>" << endl
		<< "  </form>" << endl
		<< "  already have an acount? <a href='login'>Login</a>" << endl
		<< "</body>" << endl
		<< "</html>" << endl;

	res->setBody(body.str());
	return res;
}
