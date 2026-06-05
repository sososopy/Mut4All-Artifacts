
template <typename T>
concept bar = requires(const T& t) {
  { t.GetA() };
};

template <template<typename> typename T>
struct Derived : T<Derived<T>>{};


namespace {
template<typename Derived>
struct Base2 {
    void GetA() const {}
};

using B2 = Derived<Base2>;

static_assert(bar<B2>);
}
