#include "Logouthandler.hpp"
#include "defineshandlers.hpp"

using namespace std;

Response* LogoutUser::callback(Request *req)
{
	if(!utripservice->user_is_logged_in(req->getSessionId()))
		return Response::redirect("/");

	utripservice->process_logout(req->getSessionId());
	Response *res = Response::redirect("/login");
    res->setSessionId("");

	return res;
}
