#include "Address.h"

Address::Address() {
  for (size_t i = 0; i < 4; i++) {
    octets.push_back(0);
  }
}

Address::~Address() {}

void Address::set_address(std::vector<uint8_t> inputVect) {
  octets = inputVect;
}

std::string Address::print_address() {
  std::string output;
  for (size_t i = 0; i < 4; i++) {
    output += (std::to_string(octets[i]) + '.');
  }
  output = output.substr(0, output.size() - 1);
  return output;
}

const std::vector<uint8_t> Address::get_octets() { return octets; }

bool operator==(Address &lhs, Address &rhs) {
  std::vector<uint8_t> leftVector, rightVector;
  leftVector = lhs.get_octets();
  rightVector = rhs.get_octets();
  if (leftVector[0] == rightVector[0] && leftVector[1] == rightVector[1] &&
      leftVector[2] == rightVector[2] && leftVector[3] == rightVector[3]) {
    return true;
  } else {
    return false;
  }
}