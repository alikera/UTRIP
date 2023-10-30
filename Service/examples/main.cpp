#include <iostream>
#include "Utrip.hpp"
#include "Welcomehandler.hpp"
#include "Signuphandlers.hpp"
#include "Loginhandlers.hpp"
#include "Logouthandler.hpp"
#include "Homehandlers.hpp"
#include "Wallethandlers.hpp"
#include "defineshandlers.hpp"

#include "../server/server.hpp"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) 
{
  try {
    Server server(8080);

    Utrip source;
    source.read_from_hotels_file(argv[1]);

    server.setNotFoundErrPage("static/404.html");

    server.get("/", new WelcomePage(&source));
    server.get(SIGNUP_PATH, new ShowSignupPage(&source));
    server.post(SIGNUP_PATH, new CreateUser(&source));

    server.get(LOGIN_PATH, new ShowLoginPage(&source));
    server.post(LOGIN_PATH, new LoginUser(&source));

    server.get(LOGOUT_PATH, new LogoutUser(&source));

    server.get(HOME_PATH, new Home(&source));
    server.get(APPLY_FILTER_PATH, new ApplyFilter(&source));
    server.get(HOTEL_INFO_PATH, new HotelInfo(&source));
    server.get(WALLET_PATH, new ShowWalletCredit(&source));
    server.post(WALLET_PATH, new ChargeWalletCredit(&source));

    server.run();
  } catch (Server::Exception e) {
    cerr << e.getMessage() << endl;
  }
}
