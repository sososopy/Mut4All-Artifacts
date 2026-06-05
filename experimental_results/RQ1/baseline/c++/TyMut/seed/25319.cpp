

#include <memory>

void* allocate(std::size_t n) { return std::allocate<char>().allocate(n); }

