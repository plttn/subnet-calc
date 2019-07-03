#pragma once
#include <vector>
#include <string>

class Address
{
public:
	Address();
	~Address();
	void set_address(std::vector<uint8_t>);
	std::string print_address(); //for pretty printing
	const std::vector<uint8_t> get_octets(); //for bitwise

	
private:
	std::vector<uint8_t> octets;

	friend bool operator==(Address &lhs, Address &rhs);
};

