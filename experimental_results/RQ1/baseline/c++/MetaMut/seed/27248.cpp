
#include <type_traits>

#ifndef TRIGGER_ICE
#define TRIGGER_ICE 1
#endif

#if TRIGGER_ICE

struct slip {
  template<bool C> static constexpr bool condition() { return C; }
};

template<typename std::enable_if<slip::condition<bool(true)>(), int>::type = 0>
static bool dispatch() { return true; }

bool test() {
  return dispatch();
}

#else

// No ICE if the condition function is at namespace scope.

template<bool C> static constexpr bool noslip_condition() { return C; }

template<typename std::enable_if<noslip_condition<bool(true)>(), int>::type = 0>
static bool dispatch() { return true; }

bool test() {
  return dispatch();
}

#endif

