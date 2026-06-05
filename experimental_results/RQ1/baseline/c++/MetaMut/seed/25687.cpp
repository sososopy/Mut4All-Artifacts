
class Base
{
public:
 constexpr Base() : v(1) {};
 int v;
};

class Derived : public Base
{
public:
 constexpr Derived() : Base() {};
 virtual void function();
};
