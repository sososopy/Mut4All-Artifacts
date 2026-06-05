
#include <stdio.h>

namespace MyNamespace
{
	class MyClass
	{
	public:
		MyClass() { }
		~MyClass() { }
	};
}

class MyNamespace::MyClass;

int main()
{
	fprintf(stderr, "Test code to reproduce an issue when compiling with Clang trunk\n");
	return 0;
}

