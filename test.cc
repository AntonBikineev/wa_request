#include "request.hpp"
#include <unistd.h>
#include <iostream>
#include <future>
#include <string>

int main()
{
  wa::detail::async_sender s{"1F1(0.1, 0.3)", wa::detail::get_result_handler{}};
  std::cout << "yep\n";
  std::cout << s.get<std::string>() << std::endl;

  return 0;
}
