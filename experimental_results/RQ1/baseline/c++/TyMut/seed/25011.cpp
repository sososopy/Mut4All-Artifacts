
class Base {
  protected:
    Base(int) {}
};

class Derived : public Base {
  public:
    using Base::Base;
};

template <typename DERIVED>
class MyCrtp {
  protected:
    template <typename T>
      MyCrtp(T&&) { }
};

class Implementer : public MyCrtp<Implementer> {
  using BASE = MyCrtp<Implementer>;
  public:
    using BASE::MyCrtp;
};

int main()
{
  Derived d(3);     // Compile error with GCC and Clang
  Implementer i(3); // GCC gives NO error
}


