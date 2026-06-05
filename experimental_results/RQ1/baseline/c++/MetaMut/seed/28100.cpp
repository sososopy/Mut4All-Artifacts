
  template<class>
  constexpr bool always_false = false;

  template<class T>
  struct S {
    void f() {
      static_assert(always_false<T>);
    }
  };

  template<class>
  concept True = true;

  template<class T>
  concept C = requires(T t) {
  #ifndef WORKAROUND
      { t.f() } -> True;
  #else
      t.f();
      requires True<decltype(t.f())>;
  #endif
  };

  int main() {
      static_assert(C<S<void>>);
  }

