

#include <chrono>
using namespace std::literals;

#if defined SHOW_BUG
auto x = 1'23s
#else
auto x = 123s;
#endif

