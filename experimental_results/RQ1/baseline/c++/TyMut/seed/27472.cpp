
namespace std {
template <typename _Result> struct coroutine_traits {
  using promise_type = _Result::promise_type;
};
template <typename = void> struct coroutine_handle;
template <> struct coroutine_handle<> {};
template <typename> struct coroutine_handle : coroutine_handle<> {};
struct A {
  void await_ready();
  void await_suspend(coroutine_handle<>);
  void await_resume();
};
} // namespace std
struct B {
  struct C {
    void await_ready();
    template <typename Promise>
    void await_suspend(std::coroutine_handle<Promise>);
    void await_resume();
  };
  std::coroutine_handle<> __trans_tmp_1;
  auto get_return_object() { return __trans_tmp_1; }
  auto initial_suspend() { return std::A(); }
  auto final_suspend() { return C(); }
  void unhandled_exception();
};
struct D {
  using promise_type = B;
  std::coroutine_handle<> _handle;
  void await_ready();
  void await_suspend(std::coroutine_handle<>);
  void await_resume();
};
D foo() { co_await foo(); }
