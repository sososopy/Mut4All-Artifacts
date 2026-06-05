
#include <stdio.h>

class ITest1
{
public:
	virtual void test11(void) = 0;
};

class ITest2
{
public:
	virtual void test21(void)
	{
		printf("Calling test21 %p\n", this);
	}
};

#define OPTIMIZE_SIZE __attribute__((optimize("O0")))

class Test : public ITest1, public ITest2
{
public:
	Test() : m_test(0)
	{ }

	OPTIMIZE_SIZE void init()
	{
		m_test = 4;
		printf("Init %p \n", this);
		printf("m_test = %d\n", m_test);

		// All following functions have strange 'this' pointer value.
		test11();
		test21();
	}


	void test11(void)
	{
		printf("Calling test11 %p\n", this);
		//printf("m_test = %d \n", m_test);
	}

	void test12(void)
	{
		printf("Calling test12 %p\n", this);
	}


private:
	int m_test;
};

int main()
{
	Test *test = new Test();
	test->init();
}

