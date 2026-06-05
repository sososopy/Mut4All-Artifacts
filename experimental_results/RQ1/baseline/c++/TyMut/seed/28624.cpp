
#include <coroutine>

class Task : public std::suspend_never {
 private:
  class Promise {
   public:
    Task get_return_object() {
      return Task();
    }

    std::suspend_never initial_suspend() {
      return {};
    }

    std::suspend_never final_suspend() noexcept {
      return {};
    }

    void return_void() {}

    void unhandled_exception() {}
  };

 public:
  using promise_type = Promise;
};

inline constexpr int getBarFromFoo(int foo) {
  return 0;
}

class MyClass {
  int myFoo;

  Task doSth();
};

Task MyClass::doSth() {
  co_await [&, this]() -> Task {
    co_await std::suspend_never();
    int bar = getBarFromFoo(myFoo);
  }();
}
