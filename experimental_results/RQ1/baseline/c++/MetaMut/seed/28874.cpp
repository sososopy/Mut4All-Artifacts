

template <typename> struct A
{
  struct B
  {
    template <typename T> using C = T;
    using D = C<A>;
  };
};

