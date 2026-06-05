
#include <chrono>

class foo { std::chrono::high_resolution_clock clock;};

static_assert(!decltype(foo::clock)::is_steady, "xxx");

This doesn't:

class foo {
    class bar {typedef int foo;};
};

int main()
{
    decltype (foo::bar) y;
}

