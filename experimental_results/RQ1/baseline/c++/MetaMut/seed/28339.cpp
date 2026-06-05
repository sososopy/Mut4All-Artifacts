

template<class>
struct foo {
  int operator()(...) const;
};

template<class T>
struct bar : foo<T> {
  auto operator()() const {
    return [&](auto x) {
      return foo<T>::operator()(decltype(x){});
    };
  }
};

bar<int> b;
int i = b()(0);
