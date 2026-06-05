
template <bool> using Void = void;

template <typename U> bool Init (U);

template <typename> bool VAR = Init ([] {});

template <typename T>
Void<false && VAR<T>> Foo (T)
{}

void q ()
{
  Foo ([] {});
}

