struct A { };

template<bool B>
  struct Builder
  {
    static A build() { return A(); }
  };


template<A (*F)()>
  A f()
  {
    return Builder<F != nullptr>::build();
  }

A g();

int main()
{
  f< &g >();            // OK
  f< nullptr >();       // OK
  f< &f<nullptr> >();   // ERROR
}

