
#include <iostream>
#include <utility>

#ifdef __clang__
#include <experimental/coroutine>
namespace std {
  using namespace std::experimental;
}
#else
#include <coroutine>
#endif

class promise;

class task {
 public:
  using promise_type = promise;
  using handle_type = std::coroutine_handle<promise_type>;

  explicit task(handle_type handle) : handle_(handle) {}
  ~task() { if (handle_) handle_.destroy(); }
  
  task(task&& other) : handle_(std::exchange(other.handle_, nullptr)) { }
  task& operator=(task&& other) {
    if (handle_) handle_.destroy();
    handle_ = std::exchange(other.handle_, nullptr);
    return *this;
  }

 private:
  handle_type handle_;

  task(const task&) = delete;
  task& operator=(const task&) = delete;
};

class promise {
 public:
  promise() = default;
  ~promise() = default;

  std::suspend_never initial_suspend() noexcept { return {}; }
  std::suspend_always final_suspend() noexcept { return {}; }
  void unhandled_exception() noexcept { abort(); }

  task get_return_object() {
    return task{std::coroutine_handle<promise>::from_promise(*this)};
  }

  void return_value(int rv) {
    std::cout << rv << std::endl;
  }

 private:
  promise(const promise&) = delete;
  promise operator=(const promise&) = delete;
};

struct suspend_never_with_int {
  bool await_ready() { return true; }
  void await_suspend(std::coroutine_handle<>) {}
  int await_resume() { return 42; }
};

suspend_never_with_int foo() {
  std::cout << "NOTREACHED" << std::endl;
  return {};
}

task failcase() {
  co_return false ? co_await foo() : 1;
}

int main() {
  failcase();
  return 0;
}
