
#if __has_include(<coroutine>)
#include <coroutine>
namespace stdx {
using std::coroutine_handle;
using std::noop_coroutine;
using std::suspend_always;
using std::suspend_never;
}  // namespace stdx
#elif __has_include(<experimental/coroutine>)
#include <experimental/coroutine>
namespace stdx {
using std::experimental::coroutine_handle;
using std::experimental::noop_coroutine;
using std::experimental::suspend_always;
using std::experimental::suspend_never;
}  // namespace stdx
#else
#error "coroutines unsupported"
#endif

class Task {
 public:
  struct promise_type {
    Task get_return_object() { return Task{}; }
    stdx::suspend_always initial_suspend() { return {}; }
    stdx::suspend_always final_suspend() noexcept { return {}; }
    void unhandled_exception() {}
    void return_void() {}
  };

  bool await_ready() const { return false; }
  void await_suspend(stdx::coroutine_handle<void> continuation) {}
  void await_resume() {}
};

class NonMoveableTask {
 public:
  NonMoveableTask() = default;
  NonMoveableTask(const NonMoveableTask&) = delete;
  NonMoveableTask(NonMoveableTask&&) = delete;

  NonMoveableTask& operator=(const NonMoveableTask&) = delete;
  NonMoveableTask& operator=(NonMoveableTask&& other) = delete;

  bool await_ready() const { return false; }
  void await_suspend(stdx::coroutine_handle<void>) {}
  void await_resume() {}
};

Task Foo(NonMoveableTask* task) { co_await* task; }

int main() {}
