
  template <typename T>
  struct A {};

  template <typename T>
  struct B : A<typename T::type> {};

  class C {
  private:
    using type = int;
  public:
    friend B<C>;
  };

  template struct B<C>;
