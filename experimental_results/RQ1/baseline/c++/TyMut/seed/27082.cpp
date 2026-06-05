

template <class T, class U>
concept bool C = true;

template <class T>
struct A {};

void f(A<C<int>>) {} // Error ("void f(A<C<int> >)" works fine)

