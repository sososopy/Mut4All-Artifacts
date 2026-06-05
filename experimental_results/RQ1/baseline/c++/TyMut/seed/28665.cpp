
template<auto & TASK, int, typename... ARGS> auto f1(ARGS &&...);

template<class T, int>
struct F { };

template<class T, class C>
struct K { };

template<typename T>
struct N {
  using i = typename T::i;
  struct M { };
  static void S(typename F<K<M, i>, 1>::template A<2> m) { }
  void f2() {
#ifdef BUG
    f1<N<T>::S, int>(this->t);
#else
    f1<S, int>(this->t);
#endif
  }
};

