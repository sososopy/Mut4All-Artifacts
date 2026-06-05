
#include <fmt/core.h>

#include <coroutine>
#include <functional>

#ifdef __clang__
namespace std::experimental {
using std::coroutine_handle;
using std::coroutine_traits;
}  // namespace std::experimental
#endif

struct task {
  struct promise_type;
  using handle_type = std::coroutine_handle<promise_type>;

  struct promise_type {
    auto initial_suspend() noexcept { return std::suspend_always{}; }
    auto final_suspend() noexcept { return std::suspend_always{}; }
    void return_void() noexcept {}
    task get_return_object() noexcept {
      return task{handle_type::from_promise(*this)};
    }
    void unhandled_exception() noexcept {}
  };

  task(handle_type h) : h{h} {}
  ~task() {
    if (h) {
      fmt::print("destroying coroutine\n");
      h.destroy();
      fmt::print("coroutine destroyed\n");
    }
  }

  bool await_ready() const noexcept { return false; }
  bool await_suspend(std::coroutine_handle<>) noexcept {
    h.resume();
    return true;
  }
  void await_resume() noexcept {}

  void resume() { h.resume(); }

  handle_type h;
};

struct S {
  S() { fmt::print("S::S()\n"); }
  S(const S&) { fmt::print("S::S(const S&)\n"); }
  S(S&&) { fmt::print("S::S(S&&)\n"); }
  ~S() { fmt::print("S::~S()\n"); }
};

task g(std::function<void()>) {
  fmt::print("in g()\n");
  co_return;
}

task f() {
  fmt::print("calling g()\n");
  co_await g([s=S{}] {});
  fmt::print("g called\n");
}

int main() {
  auto task = f();
  fmt::print("resuming f\n");
  task.resume();
  fmt::print("resuming f\n");
  task.resume();
}

