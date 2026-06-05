
  module M:part;

  struct A {
    template <typename> struct R;
    template <typename T> requires false struct R<T>;
    template <typename T> requires true struct R<T>;
  };

  template <typename T> int foo(T, int);
  template <typename T> int foo(T, A);

  void go() { foo(0, 0); }



  // a.cpp
  module;
  #include <format>
  export module M:a;

  // b.cpp
  module;
  #include <format>
  export module M:b;

  // m.cpp
  export module M;
  export import :a;
  export import :b;
---------------------------------------------------------

