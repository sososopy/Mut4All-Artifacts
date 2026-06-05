
module;

import <iostream>;
import <string_view>;
export module Hello;

export inline void SayHello(std::string_view name)
{
	std::cout << "Hello " << name << "!\n";
}

//main.cc
import Hello;

int main()
{
	SayHello("sdgsdg");
}

