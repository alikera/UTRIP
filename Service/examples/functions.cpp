#include "functions.hpp"
#include "defines.hpp"
#include "NotFound.hpp"
#include "BadRequest.hpp"
#include <iostream>
#include <sstream>

using namespace std;

string disassemble_input(stringstream& ss_inputs)
{
	string word;
	getline(ss_inputs, word, SPACE);
	if(word == EMPTY)
		throw BadRequest();
	return word;
}

void check_for_qmark(stringstream& ss_inputs)
{
	string command = disassemble_input(ss_inputs);
	if(command == EMPTY)
		throw BadRequest();

	if(command != QMARK)
		throw NotFound();
}

bool logged_in(User* user)
{
	if(user->get_status() == LOGGED_IN)
		return true;

	return false;
}

bool compare_uid(Hotel* h1, Hotel* h2)
{
	return h1->get_uid() < h2->get_uid();
}


unsigned long int generate_password_hash(string password)
{
    return hash<string>() (password);
}
