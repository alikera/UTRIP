#include <string>
#include <sstream>
#include <iostream>
#include "Homehandlers.hpp"
#include "defineshandlers.hpp"

using namespace std;

Response* ApplyFilter::callback(Request *req)
{
	int min_star = stoi(req->getQueryParam("from"));
	int max_star = stoi(req->getQueryParam("to"));

	utripservice->apply_star_filter(min_star, max_star, req->getSessionId());
	return Response::redirect("/home");
}

Response* HotelInfo::callback(Request *req) {
	if(!utripservice->user_is_logged_in(req->getSessionId()))
		return Response::redirect("/");

	Response *res = new Response;
	res->setHeader("Content-Type", "text/html");

	Hotel* hotel = utripservice->get_hotel(stoi(req->getQueryParam("id")));

	ostringstream body;
	body<< "<style>" << endl
		<< "li {" << endl
		<< "    display: inline;" << endl << "}" << endl
		<< "</style>" << endl
		<< "    	<ul>" << endl
	    << "		  <li><a href='home'>Home</a></li>" << endl
	    << "		  <li><a href='logout'>Logout</a></li>" << endl
	    << "        </ul>" << endl;
	body << hotel->get_hotel_info_in_html();

	res->setBody(body.str());
	return res;
}

Response* Home::callback(Request *req)
{
	if(!utripservice->user_is_logged_in(req->getSessionId()))
		return Response::redirect("/");

	Response *res = new Response;
	res->setHeader("Content-Type", "text/html");
	hotels_t all_hotels = utripservice->get_all_hotels(req->getSessionId());

	ostringstream body;
	body
		<< "<html>" << endl
	    << "<head><title>Home</title>" << endl
	    << "<style>" << endl
		<< "li {" << endl
		<< "    display: inline;" << endl << "}" << endl
		<< "</style>" << endl
	    << "</head>" << endl
	    << "    <body>" << endl	
	    << "    	<ul>" << endl
	    << "		  <li><a href='wallet'>My Credit</a></li>" << endl
	    << "		  <li><a href='logout'>Logout</a></li>" << endl
	    << "        </ul>" << endl
	    << "  		<form action='applyFilter' method='GET'>" << endl
	    << "            Filter by Star Rating: <br>" << endl
	    << "    		From: <input type='number' name='from' min='1' max='5'/> " << endl
	    << "    		To: <input type='number' name='to' min='1' max='5'/><br/>" << endl
	    << "    		<input type='submit' value='Apply' onclick='myFunction()'/>" << endl
	    << "  		</form>" << endl
	    << "<script>" << endl
		<< "function myFunction() { " << endl
  		<< "alert('Filter Applied Succesfully'); " << endl
		<< "}" << endl << "</script>" << endl;
    for(int i = 0; i < all_hotels.size(); i++) 
    {
	    body
	        << "      <img src='" << all_hotels[i]->get_image_url() << "' width='100' height='100' style='float:left'" << endl
			<< "           <p><a href='hotelInfo?id=" << all_hotels[i]->get_uid_generated() << "'>" << all_hotels[i]->get_name() << "</a></p>"
	        << "           <p> hotel star rating: " << all_hotels[i]->get_stars() << "</p>" << endl
	        << "           <p> city: " << all_hotels[i]->get_city() << "</p>"
	        << "      <br> <br> <br>";
    }
	body
		<< "    </body>" << endl
	   	<< "</html>" << endl;

	res->setBody(body.str());
	return res;
}

