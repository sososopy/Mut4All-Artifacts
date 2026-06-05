

struct Awaitable {
  int v;
  Awaitable (int _v) : v(_v) {}
  bool await_ready() { return false; }
  void await_suspend(std::coroutine_handle<coro1::promise_type>) {}
  int await_resume() { return v; }
  auto operator co_await() { return *this; }
};

coro1
my_coro
(int x)
{
  int sum = 0;
  for (unsigned i = 0; i < 100; ++i) {
    sum += co_await Awaitable{x+1};
  }
  co_return sum;
}
