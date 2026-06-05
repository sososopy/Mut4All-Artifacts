
// test.cc

#include <cstdint>
#include <cstdlib>

namespace MyISA {

typedef struct {
	uint8_t data[64];
} VecReg;

}

void foo(MyISA::VecReg &v);

using MyISA::VecReg;

class A
{
	VecReg v;
public:
	void test();
};

typedef MyISA::VecReg VecReg;

void A::test()
{
	foo(v);
}

int main()
{
	A a;
	a.test();
}

// foo.cc

#include <cstdint>

namespace MyISA {

typedef struct {
	uint8_t data[64];
} VecReg;

}

void foo(MyISA::VecReg &v)
{
	v.data[0] = 42;
}

