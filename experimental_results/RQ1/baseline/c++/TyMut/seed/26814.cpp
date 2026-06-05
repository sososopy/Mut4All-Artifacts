
#include <type_traits>

struct Member {};
struct A { Member x; };
A MakeA();

static_assert(std::is_same<decltype((MakeA().x)), Member&&>::value, ""); // MSVC++ 2015
//static_assert(std::is_same<decltype((MakeA().x)), Member>::value, ""); // clang, gcc

