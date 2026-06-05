
#include <coroutine>
#include <vector>
#include <string>

struct task {
  struct promise_type {
    auto initial_suspend() noexcept { return std::suspend_always{}; }
    auto final_suspend() noexcept { return std::suspend_always{}; }
    void return_value(int) {}
    task get_return_object() { return task{}; }
    void unhandled_exception() noexcept {}
  };

  ~task() noexcept {}

  bool await_ready() const noexcept { return false; }
  void await_suspend(std::coroutine_handle<>) noexcept {}
  int await_resume() noexcept { return 1; }
};

task f(int)
{
    co_return 1;
}

task g(bool b)
{
    auto result = b ? co_await f(1) : 2;
}

