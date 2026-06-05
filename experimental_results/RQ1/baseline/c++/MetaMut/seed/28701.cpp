
#include <coroutine>

struct future {
  struct promise_type {
    std::suspend_always initial_suspend() noexcept { return {};}
    std::suspend_always final_suspend() noexcept { return {}; }
    void unhandled_exception() {}
    future get_return_object() { return {}; }
    void return_void() {}
  };
};

void failcase() {
  auto foo = [](auto&&) -> future {
    switch (42) {
      case 42:
        co_return;
    }
  };
  foo(1);
}

