#include <string>
#include <sstream>
#include <iostream>
#include "Loginhandlers.hpp"
#include "defineshandlers.hpp"


using namespace std;

Response *LoginUser::callback(Request *req)
{
    string username = req->getBodyParam(USERNAME);
    string password = req->getBodyParam(PASSWORD);

    if(utripservice->have_empty_field(username, password, "sth"))
        throw Server::Exception(EMPTY_FIELD);

    if(utripservice->login_user(username, password))
        return login_is_successful(username);

    else
        throw Server::Exception(INCORRECT_USERNAME_PASS);

}

Response *LoginUser::login_is_successful(string session_id)
{
    Response *res = Response::redirect("/home");
    res->setSessionId(session_id);

    return res;
}

Response *ShowLoginPage::callback(Request *req)
{
    if(utripservice->user_is_logged_in(req->getSessionId()))
        return Response::redirect("/home");

    Response *res = new Response;

    res->setHeader("Content-Type", "text/html");

    ostringstream body;
    body
        << "<html>" << endl
        << "<head><title>Login</title></head>" << endl
        << "<body>" << endl
        << "  <form action='login' method='POST'>" << endl
        << "    Username: <input type='text' name='username'/><br/>" << endl
        << "    Password: <input type='password' name='password'/><br/>" << endl
        << "    <input type='submit' value='Login'/>" << endl
        << "  </form>" << endl
        << "     don't have an account? <a href='signup'>Sign up</a>" << endl
        << "</body>" << endl
        << "</html>" << endl;

    res->setBody(body.str());
    return res;
}
