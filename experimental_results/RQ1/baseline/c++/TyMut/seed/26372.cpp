
module;

#include <iostream>

export module Foo;

export void
my_hello (const char *str)
{
 std::cout << str << std::endl;
}

