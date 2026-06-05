
namespace std {
  template <typename T> T &&move(T &&);
}

template <typename S> struct D {
  template <typename T> D (D<T> x) : k(&x.foo ()) {}
  S &foo ();
  int *k;
};

D<int> bar ();

struct F {
  D<int> baz () {
    D<F> f = bar ();
    return std::move (*reinterpret_cast<D<int> *> (&f));
  }
};