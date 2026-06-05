
#include <cstddef>
 
void bar(void*) {}
 
struct foo
{
	operator std::nullptr_t()
	{
		return nullptr;
	}
};

int main()
{
	bar(foo());
}

