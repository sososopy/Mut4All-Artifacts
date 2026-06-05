
  struct S {
      void bar();

      __attribute__((deprecated("use bar() instead.")))
      virtual void foo();
  };

  inline void S::foo() { bar(); }

  int main()
  {
      return 0;
  }
