
#include <iostream>

enum class Month {jan=1, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec};

std::ostream& operator<<(std::ostream& os, Month m)
{
  return os << m;                    // BUG - SEGMENTATION FAULT
}


int main()
{
  Month m = Month::may;
  std::cout << m << '\n';
  return 0;
}

