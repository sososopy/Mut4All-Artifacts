


//hello.cc

module;
#include<cstdio>
export module hello;
export inline void greeter (char const* cstr) noexcept
{
	std::puts(cstr);
}

//main.cc

import hello;
int main (void)
{
	greeter("Hello\n");
}
