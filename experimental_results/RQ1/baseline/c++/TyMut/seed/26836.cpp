
typedef int F ();

F foo;

template <F&> struct A { };
A<foo> a;
