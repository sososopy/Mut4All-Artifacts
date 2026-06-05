class B
  {
    template <typename T> friend struct A;
  private:
      B(int, char) {}
      ~B() {}
  };

  template <typename T>
  struct A
  {
      T t;
      template<typename... Args>
      A(Args&&... args) noexcept(noexcept(T(std::forward<Args>(args)...)))
        : t(std::forward<Args>(args)...)
      {}
  };

  struct C {
    A<B> delegate;

    template<typename... Args>
    C(Args&&... args)
        noexcept(noexcept(A<B>(std::forward<Args>(args)...)))
        : delegate(std::forward<Args>(args)...)
      { }
  };

  #endif

  int main() {
    A<B> ab(42, 'f'); // Works
    C c(42, 'f'); // Fails

    return 0;
  }