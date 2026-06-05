
void no_coroutine_traits() {
  co_await 4;
}

template <class... Args>
struct void_t_imp {
  using type = void;
};
