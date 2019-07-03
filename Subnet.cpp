#include "Subnet.h"

Subnet::Subnet()
{
}

Subnet::~Subnet()
{
}

void Subnet::load_subnet(std::string input) {
	char tokenizable[30];
	char * tok;
	uint8_t intTok;
	int maskLength; 
	std::vector<uint8_t> inputAddress;
	std::vector<uint8_t> maskAddress;

	input.copy(tokenizable, 30);	
	tok = strtok(tokenizable, "./");
	for (size_t i = 0; i < 4; i++) //fill octets
	{
		intTok = atoi(tok);
		inputAddress.push_back(intTok);
		tok = strtok(NULL, "./");
	}
	maskLength = atoi(tok);
	this->inputAddress.set_address(inputAddress); //now this->inputaddress contains address in vector
	//build mask address
	maskAddress = convert_cidr(maskLength);
	this->maskAddress.set_address(maskAddress);
	
	this->subnetAddress.set_address(get_subnet());
	
	this->subnetBC.set_address(get_subnet_broadcast());

	this->classfulAddress.set_address(get_classful());

	get_usable_nums(maskLength);

	get_addr_range();

	this->isValid = get_valid_status();

}

std::vector<uint8_t> Subnet::convert_cidr(int cidr) {
	//fill up starting from left using 8 bits at a time
	std::vector<uint8_t> maskVect;

	for (size_t i = 0; i < 4; i++)
	{
		if (cidr>=8)
		{
			cidr -= 8;
			maskVect.push_back(255);
			
		}
		else if (cidr > 0)
		{
			int multiplier = 8 - cidr;
			int partial = pow(2, cidr) - 1;
			partial = partial << multiplier; //account for subtracting effectively giving you lower end of octet, so shift over by number its short by to get it on MSB
			maskVect.push_back(partial);
			cidr -= 8;
		}
		else
		{
			maskVect.push_back(0);
		}
	}

	return maskVect;
}

std::vector<uint8_t> Subnet::get_subnet() {
	std::vector<uint8_t> subnet;
	std::vector<uint8_t> inOcts, maskOcts;
	uint8_t hostOct;


	inOcts = this->inputAddress.get_octets();
	maskOcts = this->maskAddress.get_octets();
	for (size_t i = 0; i < 4; i++)
	{ 
		hostOct = inOcts[i] & maskOcts[i];
		subnet.push_back(hostOct);
	}

	return subnet;
}

std::vector<uint8_t> Subnet::get_subnet_broadcast() {
	std::vector<uint8_t> subnetBC;
	std::vector<uint8_t> subnetMask = this->maskAddress.get_octets();
	std::vector<uint8_t> inOcts, maskOcts;
	uint8_t bcOct;


	inOcts = this->inputAddress.get_octets();
	maskOcts = this->maskAddress.get_octets();


	for (size_t i = 0; i < 4; i++)
	{
		subnetMask[i] = ~subnetMask[i]; //broadcast will be subnet flipped
	}
	for (size_t i = 0; i < 4; i++)
	{
		bcOct = subnetMask[i] | inOcts[i]; //then OR with octet
		subnetBC.push_back(bcOct);
	}

	return subnetBC;
}

std::vector<uint8_t> Subnet::get_classful() {
	std::vector<uint8_t> classfulAddress;
	std::vector<uint8_t> network = this->inputAddress.get_octets();
	std::vector<uint8_t> classMask;
	std::vector<uint8_t> classfulBC;

	for (size_t i = 0; i < 4; i++) //fill mask up for manipulating
	{
		classMask.push_back(0);
	}

	if (network[0] >= 0 && network[0] <= 127) //class a
	{
		this->classType = 'A';
		classMask[0] = 255;
		
	}
	else if (network[0] >= 128 && network[0] <= 191) //b
	{
		this->classType = 'B';
		classMask[0] = 255;
		classMask[1] = 255;
	}
	else if (network[0] >= 192 && network[0] <= 223) //c
	{
		this->classType = 'C';
		classMask[0] = 255;
		classMask[1] = 255;
		classMask[2] = 255;
	}
	else //d or e (unclear how to handle)
	{
		this->classType = 'x';
	}
	
	for (size_t i = 0; i < 4; i++)
	{
		classfulAddress.push_back(network[i] & classMask[i]);
	}

	for (size_t i = 0; i < 4; i++)
	{
		classMask[i] = ~classMask[i];
	}
	for (size_t i = 0; i < 4; i++)
	{
		classfulBC.push_back(classfulAddress[i] | classMask[i]);
	}
	this->classfulBC.set_address(classfulBC);
	return classfulAddress;
}

void Subnet::get_usable_nums(int maskLength) {
	int subnets, hosts;
	
	hosts = (pow(2, (32 - maskLength))) - 2;
	this->numHosts = hosts;
	subnets = 1;

	if (this->classType == 'A')
	{
		subnets = pow(2, (maskLength - 8)) - 2;
	}
	else if (this->classType == 'B')
	{
		subnets = pow(2, (maskLength - 16)) - 2;
	}
	else if (this->classType == 'C')
	{
		subnets = pow(2, (maskLength - 24)) - 2;
	}

	if (subnets == -1) {
		subnets = 1;
	}

	this->numSubnets = subnets;
	return;
}

void Subnet::get_addr_range() {
	std::vector<uint8_t> lowAddr, highAddr;
	std::vector<uint8_t> bcAddr = this->subnetBC.get_octets();
	std::vector<uint8_t> netAddr = this->subnetAddress.get_octets();


	netAddr[3] += 1; //get first host
	bcAddr[3] -= 1;//get last host

	this->lowAddr.set_address(netAddr);
	this->highAddr.set_address(bcAddr);
}

std::string Subnet::get_valid_status() {
	if (inputAddress == subnetBC || inputAddress == subnetAddress) {
		return "no";
	} else {
		return "yes";
	}
}

void Subnet::print_subnet() {
	std::cout << "Input Address: " << this->inputAddress.print_address() << std::endl;
	std::cout << "Classful Net Address: " << this->classfulAddress.print_address() << std::endl;
	std::cout << "Classful Broadcast: " << this->classfulBC.print_address() << std::endl;
	std::cout << "Subnet Address: " << this->subnetAddress.print_address() << std::endl;
	std::cout << "Subnet Mask: " << this->maskAddress.print_address() << std::endl;
	std::cout << "Subnet Broadcast: " << this->subnetBC.print_address() << std::endl;	

	std::cout << "Class: ";
	if (this->classType == 'x') {
		std::cout << "N/A" << std::endl;
	} else {
		std::cout << this->classType << std::endl;
	}
	std::cout << "Number of usable hosts per subnet: " << this->numHosts << std::endl;
	std::cout << "Number of usable subnets for class: " << this->numSubnets << std::endl;
	std::cout << "Host range: " << this->lowAddr.print_address() << "-" << this->highAddr.print_address() << std::endl;
	std::cout << "Valid host?: " << this->isValid << std::endl;
}