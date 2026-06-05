
#include <iostream>
#include <locale>

inline int operator<<(void*,       const std::locale&) { return 1; }
inline int operator>>(void*,       const std::locale&) { return 1; }
inline int operator<<(bool,        const std::locale&) { return 2; }
inline int operator>>(bool,        const std::locale&) { return 2; }

int main(int argc, char *argv[])
{

          std::locale loc;
          if((std::cin  >> loc)!=1)
             return 1;
          if((std::cout << loc)!=1);
             return 1;

          return 0;
}

