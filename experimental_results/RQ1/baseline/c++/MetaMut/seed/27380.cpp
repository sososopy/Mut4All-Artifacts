
module;
#include <iostream>
#include <string_view>
export module hello;
export inline void greeter (std::string_view name) noexcept
{
  std::cout << "Hello " << name << "!\n";
}
