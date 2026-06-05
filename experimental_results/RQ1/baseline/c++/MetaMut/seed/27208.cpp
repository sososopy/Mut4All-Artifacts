
#include <coroutine>

struct simple {
  static inline int alive = 0;
  simple() { ++alive; }
  simple(simple&&) { ++alive; }
  ~simple() { --alive; }

  struct promise_type {
    simple get_return_object() { return simple{}; }
    void return_void() {}
    void unhandled_exception() {}
    auto initial_suspend() noexcept { return std::suspend_never{}; }
    auto final_suspend() noexcept { return std::suspend_never{}; }
  };
};

simple f() { co_return; }

int main() {
  {
    f();
  }
  return simple::alive;
}

