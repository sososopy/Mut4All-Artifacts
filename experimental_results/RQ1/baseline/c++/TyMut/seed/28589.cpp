
namespace std {
template <class> class initializer_list {
  int *a;
  decltype(sizeof 0) b;
};
}
struct C { C(); };
struct D { D(std::initializer_list<C>); };
namespace std {
template <typename e, typename> struct coroutine_traits : e {};
template <typename = void> struct coroutine_handle {
  operator coroutine_handle<>();
};
}
struct F {
  void await_ready();
  void await_suspend(std::coroutine_handle<>);
  void await_resume();
};
struct M {
  void await_ready() noexcept;
  template <typename h> void await_suspend(h) noexcept;
  void await_resume() noexcept;
};
struct I {
  F initial_suspend();
  auto final_suspend() noexcept { return M{}; }
};
struct K {
  struct J : public I {
    void unhandled_exception();
//    K get_return_object() { return K{}; }
  };
  using promise_type = J;
};
struct Q {
  Q(int);
  void await_ready();
  void await_resume();
  void await_suspend(std::coroutine_handle<>);
};
Q foo (C, D);
struct L {
  K bar () { co_await foo (C(), {C()}); }
};

