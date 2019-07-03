#include "Subnet.h"
#include <iostream>

int main() {
	Subnet host;
	std::string inputString;

	std::cout << "Enter an IP address/mask in the format 192.168.1.1/24: ";
	std::cin >> inputString;

	host.load_subnet(inputString);
	host.print_subnet();
}