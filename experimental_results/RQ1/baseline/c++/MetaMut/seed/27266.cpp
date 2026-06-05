
#include <cstring>

struct Code
{
    constexpr Code(int) noexcept : _buffer{'1', '2', '\0'} {}

    char _buffer[3];
};

const Code T1 = {1};
const Code T2 = Code{1};
const Code T3 = T1;
const Code T4 = T2;
const Code T5{1};
const Code T6 = T5;

#define TEST T1

int
foo(const char* x)
{
  return std::strcmp(TEST._buffer, x);
}

size_t
bar()
{
  return std::strlen(TEST._buffer);
}
