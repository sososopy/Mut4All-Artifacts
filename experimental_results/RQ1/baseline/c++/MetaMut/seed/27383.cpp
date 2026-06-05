
template <typename T, bool B = false>
struct F
{
  static_assert(B);
};

struct S
{
  template <typename T, int = F<T>::k> operator T();
};

void f(int&);
void f(S);

void foo()
{
  f(S{});
}
