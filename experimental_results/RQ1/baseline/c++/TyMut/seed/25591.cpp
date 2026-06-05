
#include <vector>
#include <iostream>

template <typename T>
class QForeachContainer
{
public:
	inline QForeachContainer(const T& t)
	: c(t), brk(0), i(c.begin()), e(c.end())
	{
	}
	const T c;
	int brk;
	typename T::const_iterator i, e;
};

int main()
{
	std::vector<int> v(10);
	for (QForeachContainer<__typeof__(v)> _container_(v);
	     !_container_.brk && _container_.i != _container_.e;
	     __extension__({ ++_container_.brk; ++_container_.i; })
	    )
		for (int val = *_container_.i;
		     ;
		     __extension__({--_container_.brk; break; })
		    )
			std::cout << val << " ";
}
