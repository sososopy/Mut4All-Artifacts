
#include <iostream>
#include <tuple>

using namespace std;

constexpr long index(long i, long size)
{
    return (i<0)?i:(i+size);
}

template<long i, typename ...T>
struct at_index{
	const tuple<T...>* p;
	typedef decltype(get<i>(*p)) type;
};

template<long i, typename ...T>
typename at_index<index(i, sizeof...(T)), T...>::type at(const tuple<T...>& x)
{
	return get<index(i,sizeof...(T))>(x);
}

int main()
{
   tuple<long, long> a;
   cout << at<-1>(a) << endl;
}
