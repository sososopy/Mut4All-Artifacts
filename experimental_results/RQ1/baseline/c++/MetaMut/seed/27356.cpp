
#include <cstddef>

constexpr int
fn1 ()
{
  unsigned char foo;
  unsigned char u = foo; // OK: u has an indeterminate value
  return u; // UB: copy-init -> standard conversion to int
}

constexpr int
fn2 ()
{
  unsigned char foo;
  int i = foo; // UB
  return 0;
}

constexpr int
fn3 ()
{
  unsigned char foo;
  char8_t u = foo; // UB: char8_t not an unsigned ordinary character type
  return 0;
}

constexpr int
fn4 ()
{
  std::byte foo;
  std::byte b = foo; // OK
  return 0;
}

constexpr int w1 = fn1 ();
constexpr int w2 = fn2 ();
constexpr int w3 = fn3 ();
constexpr int w4 = fn4 ();
