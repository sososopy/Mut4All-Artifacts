

class Base {
  protected:
    Base();
};

class Derived : public Base {
  public:
    using Base::Base;
};

Derived d;

