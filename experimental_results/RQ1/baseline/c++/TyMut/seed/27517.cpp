
class Base {
    int x;
public:
    Base(int x): x{x} {}
    virtual void f() = 0;
};

class Derived : public virtual Base  {
  public:
    Derived() = default;
};

class Concrete: public Derived {
public:
    Concrete(): Base{42} {}
    void f() override {}
};

