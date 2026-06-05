
#include <coroutine>
#include <memory>

// promise
struct promise {
  struct promise_type {
    promise get_return_object() { return {}; }
    std::suspend_never initial_suspend() { return {}; }
    std::suspend_never final_suspend() noexcept { return {}; }
    void unhandled_exception() {};
  };
};

// generate awaitable (1)
std::suspend_never func1(std::initializer_list<std::shared_ptr<int>>) { return {}; }

// generate awaitable (2)
std::suspend_never func2(std::initializer_list<int>) { return {}; }

promise task() {
  {
    // produces a compiler error
    co_await func1({ std::make_shared<int>(1) });
  }

  {
    // does not produce a compiler error
    func1({ std::make_shared<int>(1) });
  }

  {
    // does not produce a compiler error
    auto is = { std::make_shared<int>(1) };
    co_await func1(is);
  }

  {
    // does not produce a compiler error
    co_await func2({ 1 });
  }
}

int main() { task(); }


