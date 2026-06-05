
#include <functional>

struct server
{
	server()
	{
		onopen = [](){};
	}

	std::function<void ()> onopen;
};

int main()
{
}
