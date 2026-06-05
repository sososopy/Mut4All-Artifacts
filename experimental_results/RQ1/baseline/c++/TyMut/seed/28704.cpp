
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
#include <exception>
#include <iostream>
#include <utility>

class Task {
 public:
  struct promise_type {
    struct final_awaitable {
      bool await_ready() noexcept { return false; }
      auto await_suspend(stdx::coroutine_handle<promise_type> coro) noexcept {
        return coro.promise().continuation;
      }
      void await_resume() noexcept {}
    };
    Task get_return_object() {
      return Task(stdx::coroutine_handle<promise_type>::from_promise(*this));
    }
    stdx::suspend_always initial_suspend() { return {}; }
    final_awaitable final_suspend() noexcept { return {}; }
    void unhandled_exception() { std::terminate(); }
    void return_void() {}

    stdx::coroutine_handle<void> continuation = stdx::noop_coroutine();
  };

  Task(const Task&) = delete;
  Task(Task&& other) noexcept
      : handle_(std::exchange(other.handle_, nullptr)) {}
  Task& operator=(const Task&) = delete;
  Task& operator=(Task&& other) noexcept {
    handle_ = std::exchange(other.handle_, nullptr);
    return *this;
  }
  ~Task() {
    if (handle_) {
      handle_.destroy();
    }
  }

  bool await_ready() const { return false; }
  auto await_suspend(stdx::coroutine_handle<void> continuation) {
    handle_.promise().continuation = continuation;
    return handle_;
  }
  void await_resume() {}

 private:
  explicit Task(stdx::coroutine_handle<promise_type> handle)
      : handle_(handle) {}

  stdx::coroutine_handle<promise_type> handle_;
};

struct RunTask {
  struct promise_type {
    RunTask get_return_object() { return {}; }
    stdx::suspend_never initial_suspend() { return {}; }
    stdx::suspend_never final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() { std::terminate(); }
  };
};

struct Foo {
  Foo() { std::cerr << "Foo()\n"; }
  ~Foo() { std::cerr << "~Foo()\n"; }
};

Task DoAsync() {
  std::cerr << "START TASK\n";
  co_return co_await [foo = Foo{}]() -> Task {
    std::cerr << "IN LAMBDA\n";
    co_return;
  }();
}

RunTask Main() { co_await DoAsync(); }

int main() { Main(); }
