
   template <typename T> struct S {};
   template <typename T> using constS = const S<T>;
   template <typename T> void FOO(constS<T> a) {}

