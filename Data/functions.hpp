#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include "User.hpp"
#include "Hotel.hpp"

#include <string>
#include <sstream>

std::string disassemble_input(std::stringstream& ss_inputs);
void check_for_qmark(std::stringstream& ss_inputs);
bool logged_in(User* user);
bool compare_uid(Hotel* h1, Hotel* h2);
unsigned long int generate_password_hash(std::string password);

#endif