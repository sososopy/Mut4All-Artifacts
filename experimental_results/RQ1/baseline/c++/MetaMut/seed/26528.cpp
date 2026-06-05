
module;
#include <string>
export module hello;
export auto hello() -> std::string { return "Hello";  }
$ cat main.cpp 
#include <iostream>
import hello;
auto main() -> int { std::cout << hello() << ", modules!\n";  }
