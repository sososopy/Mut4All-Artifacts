
#include <coroutines>

struct promise {
  auto initial_suspend() { return std::suspend_always{}; }
  auto final_suspend() noexcept { return std::suspend_always{}; }
  void unhandled_exception();
  auto get_return_object() { return std::coroutine_handle<promise>::from_promise(*this); }
  void return_void();
};

class task {
public:
  using promise_type = promise;
  task(std::coroutine_handle<promise_type> handle);
};

task func ()
{
  co_return;
}

