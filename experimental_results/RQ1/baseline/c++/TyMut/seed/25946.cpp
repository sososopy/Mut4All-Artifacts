#include <algorithm>
#include <vector>
#include <set>

using namespace std;

template<class elements_it>
class my_iterator
{
public:
	my_iterator(
		elements_it elements_begin,
		elements_it elements_end) :
		elements_begin_(elements_begin),
		elements_end_(elements_end),
		elements_used(distance(elements_begin, elements_end))
	{
		f();
	}

	void f()
	{
		size_t &ii = i;

		for_each(elements_begin_, elements_end_, [&](typename elements_it::value_type) 
		{
			if(elements_used[ii]) return;

			g();
		});
	}
	
	void g()
	{
	}
	
private:
	elements_it elements_begin_;
	elements_it elements_end_;
	size_t i;
	vector<int> elements_used;
};
	
int main(int, char **)
{
	set<int> elements1 = { 1, 2, 3 };
	
	my_iterator<set<int>::iterator> i1(begin(elements1), end(elements1));
}

