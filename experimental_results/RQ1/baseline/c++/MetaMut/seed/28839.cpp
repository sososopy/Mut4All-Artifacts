
#include <string>


export module hello;

import "string.hxx";

export std::string format_hello (const char* n)
{
  return "Hello, " + std::string (n) + '!';
}



#include <cassert>

import "string.hxx";
import hello;

int main ()
{
  assert (format_hello ("World") == "Hello, World!");
}
