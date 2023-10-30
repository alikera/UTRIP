#include <iostream>
#include "Utrip.hpp"
#include "functions.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	Utrip source;
	source.read_from_files(argv[1], argv[2]);
	source.get_requests();
}