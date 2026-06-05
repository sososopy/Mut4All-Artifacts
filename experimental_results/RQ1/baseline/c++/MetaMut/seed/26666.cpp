
#include <iostream>
#include <sstream>

int main ()
{
   std::stringstream str2;
   str2 << "std::stringstream log message";

   std::cout << str2.str() << std::endl;   //This line works
   std::cout << str2       << std::endl;   //This line fails if -std=c++11 option is used

  return 0;
}

