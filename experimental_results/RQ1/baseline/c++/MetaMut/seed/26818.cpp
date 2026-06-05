
 #include <cstddef>
 #include <initializer_list>
 struct S {
     template<std::size_t N> S(char const (&)[N]);
 };
 struct T { S s; };
 void f(std::initializer_list<T>);
 void g() { f({{""}}); }

