
#include <iostream>
using LL = long long;

constexpr bool isPrimeR(LL n, LL c) {
  return (c * c > n) ? true : (n % c == 0) ? false : isPrimeR(n, c + 2);
}

constexpr bool isPrimeConstexpr(LL n) {
  return (n <= 1) ? false : n < 4 || (n % 2 == 1 && isPrimeR(n, 3));
}

int main() {
  {
    constexpr LL m = 1073741789LL;  // prime
    constexpr bool x = isPrimeConstexpr(m);
    std::cout << m << " is " <<  (x ? "prime" : "not prime") << '\n';
  }

