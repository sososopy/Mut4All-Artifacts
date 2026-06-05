

template<typename T>
constexpr T foo(T t)
{
  T tt = t;
  ++tt;
  return tt;
}
 
struct X
{
  X() { }
  X& operator++() { return *this; }
};
 
int main()
{
  X x;
  foo(x);
}

