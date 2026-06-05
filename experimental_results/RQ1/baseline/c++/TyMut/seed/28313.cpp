
template<typename T>
typename T::foo
f(T) { }

template<typename T>
typename T::bar
f(T) { static_assert(false, ""); }

struct X { using foo = void; };

int main()
{
  X x;
  f(x);
}


