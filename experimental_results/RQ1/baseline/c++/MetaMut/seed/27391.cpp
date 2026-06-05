
#include <cstdint>
uint64_t v;
constexpr auto p{reinterpret_cast<uint64_t>(&v) - 1u};

