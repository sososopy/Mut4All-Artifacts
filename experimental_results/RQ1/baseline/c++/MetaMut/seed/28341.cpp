
#include <coroutine>
using std::coroutine_handle;
using std::suspend_always;
using std::suspend_never;

#include <initializer_list>
#include <string_view>

struct promise_t;

struct ret_t {
  using promise_type = promise_t;
};

struct promise_t {

  ret_t get_return_object() { return {}; }

  constexpr auto initial_suspend() noexcept { return suspend_never(); }

  void return_void() {}
  auto final_suspend() noexcept { return suspend_never(); }

  void unhandled_exception()
  {
    __builtin_trap();
  }

};

auto operator co_await(ret_t)
{
  struct W {
    auto await_ready() { return std::true_type(); }
    void await_suspend(coroutine_handle<>) {}
    void await_resume() {}
  };
  return W();
}

ret_t f1(std::initializer_list<std::string_view>)
{
  co_return;
}

ret_t f2()
{
  co_await f1({"abc", "def"});
}

