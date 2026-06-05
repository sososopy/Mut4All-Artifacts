
export module math;

export auto square(const auto& x) { return x * x; }  // An abbreviated function template

export const double lambda{1.303577269034296391257};        // Conway's constant

export enum class Oddity { Even, Odd };
bool isOdd(int x) { return x % 2 != 0; }  // Module-local function (not exported)
export auto getOddity(int x) { return isOdd(x) ? Oddity::Odd : Oddity::Even; }


import <iostream>;
#define FMT_HEADER_ONLY
#include "fmt/format.h"

import math;

int main()
{
  std::cout << "Lambda squared: " << square(lambda) << std::endl;

  int number;
  std::cout << "\nPlease enter an odd number: ";
  std::cin >> number;
  std::cout << std::endl;

  switch (getOddity(number))
  {
    using enum Oddity;
  case Odd:
    std::cout << "Well done! And remember: you have to be odd to be number one!";
    break;
  case Even:
    fmt::print("Odd, {} seems to be even?", number);
    break;
  }
  std::cout << std::endl;
}
