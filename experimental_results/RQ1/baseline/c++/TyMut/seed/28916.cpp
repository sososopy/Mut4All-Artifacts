
#include <coroutine>
#include <vector>

class Task {
 public:
  class promise_type {
   public:
    Task get_return_object() { return Task(); }
    std::suspend_never initial_suspend() { return {}; }
    std::suspend_never final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() {}
  };

  Task() {}

  bool await_ready() { return true; }
  bool await_suspend(std::coroutine_handle<> handle) { return false; }\
  void await_resume() {}
};

class Parallel {
 public:
  Parallel(std::vector<Task> tasks) {}
  bool await_ready() { return true; }
  bool await_suspend(std::coroutine_handle<> handle) { return false; }
  void await_resume() {}
};

class MyClass {
 public:
  void doSomething();

 private:
  Task doNothing();
};

void MyClass::doSomething() {
  Task task = [this]() -> Task { co_await Parallel({doNothing()}); }();
}

Task MyClass::doNothing() {
  co_return;
}
