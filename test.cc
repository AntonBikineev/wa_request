#include "request.hpp"
#include <unistd.h>
#include <iostream>
#include <future>
#include <string>
#include <boost/thread/future.hpp>

std::string foo(std::string sstr)
{
  std::cout << sstr;
  return sstr;
}

int main()
{
  wa::detail::async_sender s{"BesselJ(0.1,0.2)", foo};
  std::cout << "yep\n";
  s.get<std::string>();


  return 0;
}
