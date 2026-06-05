

template <int> bool b;

template <typename> 
struct C {
  template <typename> friend int foo() noexcept(b<1>);
};

template <typename> int foo() noexcept(b<1>);

auto a = C<int>();
