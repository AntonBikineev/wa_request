#include "request.hpp"

#include <iostream>

int main()
{
  wa::request request("BesselJ (0.1, 0.5) at 50 digits", std::launch::deferred);
  std::cout << request.get() << std::endl;

  return 0;
}
