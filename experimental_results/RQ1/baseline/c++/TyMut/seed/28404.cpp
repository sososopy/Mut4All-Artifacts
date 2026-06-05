
#include <exception>

class C {
public:
	~C() {}
};

int foo()
{
	C c;

	try
	{
		return 1;
	}
	catch (std::exception const&)
	{
	}

	if (true)
		throw std::exception();
}

int main() {
    return 0;
}
