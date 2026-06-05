
namespace std {
template <typename> struct remove_reference;
template <typename _Tp> struct remove_reference<_Tp &> { typedef _Tp type; };
template <typename _Tp> remove_reference<_Tp>::type move(_Tp &&);
template <typename _Tp> class optional {
public:
  _Tp *operator->();
};
class vector {
public:
  int begin();
  int end();
};
} // namespace std
template <typename...> class future { future(future &&); };
namespace internal {
struct extract_values_from_futures_vector {
  using future_type = future<>;
  static future_type current_exception_as_future();
};
} // namespace internal
template <typename FutureIterator>
auto when_all_succeed(FutureIterator, FutureIterator) {
  using result_transform = internal::extract_values_from_futures_vector;
  return result_transform::current_exception_as_future();
}
namespace std {
template <typename...> struct coroutine_traits;
template <typename = void> struct coroutine_handle;
template <> struct coroutine_handle<> {};
template <typename> struct coroutine_handle : coroutine_handle<> {};
struct suspend_never {
  void await_ready();
  void await_suspend(coroutine_handle<>);
  void await_resume();
};
} // namespace std
namespace internal {
class coroutine_traits_base {
public:
  class promise_type {
  public:
    std::suspend_never initial_suspend();
    std::suspend_never final_suspend();
  };
};
struct awaiter {
  future<> _future;
  void await_ready();
  template <typename U> void await_suspend(std::coroutine_handle<U>);
  void await_resume();
};
} // namespace internal
auto operator co_await(future<> f) { return internal::awaiter(std::move(f)); }
namespace std {
template <typename... T, typename... Args>
class coroutine_traits<future<T...>, Args...>
    : public internal::coroutine_traits_base {};
class server {
  struct leader_state {
    vector _replicatoin_fibers;
  };
  optional<leader_state> _leader_state;
  future<> drop_leadership();
};
future<> server::drop_leadership() {
  co_await when_all_succeed(_leader_state->_replicatoin_fibers.begin(),
                            _leader_state->_replicatoin_fibers.end());
