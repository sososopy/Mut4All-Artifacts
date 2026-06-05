
template <int ...Is>
struct is1 {};

template <int ...Is>
using is2 = is1<Is...>;

template <template <int ...> class C, int ...Is, int ...Js>
void foo1(C<Is...> a, C<Js...>  b) {}

template <template <int ...> class C, int ...Is, int ...Js>
void foo2(C<Is...> a, C<Js...> &b) {}   // !

void test()
{
  is1<1,2,345> x;
  is2<6,78,90> y;
  foo1(x,y);
  foo2(x,y);
}
