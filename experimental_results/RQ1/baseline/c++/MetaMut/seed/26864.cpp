
struct VBase { };

class Base
	: public virtual VBase
{
protected:
	Base(int, int)
	{}
};

struct Derived : Base
{
	Derived()
	:
		Base{1, 1}
	{}
};

Derived myDerived;


