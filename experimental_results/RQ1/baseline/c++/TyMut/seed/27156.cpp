struct S { };

template<typename T, typename U>
auto foo(T, U)
{
  [] <> () { foo (S{}, S{}); };
}

