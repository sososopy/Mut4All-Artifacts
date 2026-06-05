
#include <algorithm>
#include <cstddef>


export module my.std;

export import "my.std.headers.hpp";


//test.cpp:

import my.std;

int main() {
  std::byte *b{};
  std::copy(b, b, b);
}
