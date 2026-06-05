
class Base { public: virtual ~Base () {} };
class Derived: virtual Base { public: virtual ~Derived() {} };
class D2: virtual public Base, public Derived { public: D2() {} ~D2() {} };

#include <cassert>
int main ()
{
        Derived *p = new D2;
        assert(dynamic_cast<Base *>(p)); // unexpected error: ‘Base’ is an inaccessible base of ‘Derived’
}

