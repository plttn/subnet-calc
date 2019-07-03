#pragma once
#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include "Address.h"
class Subnet
{
public:
	Subnet();
	~Subnet();
	void load_subnet(std::string);
	void print_subnet();

private:
	Address inputAddress;
	Address subnetAddress;
	Address maskAddress;
	Address classfulAddress;
	Address classfulBC;
	Address subnetBC;
	Address lowAddr;
	Address highAddr;
	char classType;
	std::string isValid;

	std::vector<uint8_t> convert_cidr(int);
	std::vector<uint8_t> get_subnet();
	std::vector<uint8_t> get_subnet_broadcast();
	std::vector<uint8_t> get_classful();
	void get_usable_nums(int);
	void get_addr_range();
	std::string get_valid_status();

	int numHosts;
	int numSubnets;
};
