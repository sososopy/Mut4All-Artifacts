
export module hello;
import <string_view>;
export namespace hello
{
  void say_hello (const std::string_view& name);
}


module hello;
import <iostream>;
namespace hello
{
  void say_hello (const std::string_view& n)
  {
    std::cout << "Hello, " << n << '!' << std::endl;
  }
}

