#include <iostream>
#include "Utrip.hpp"
#include "functions.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	Utrip source;
	source.read_from_file(argv[1]);
	source.get_requests();
}