
#include <functional>

int main()
{
	std::function<void()> x[] = { 
		[x](){} 
	};
}

