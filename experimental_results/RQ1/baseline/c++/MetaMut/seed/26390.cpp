
#include <utility>

using std::integer_sequence;
using std::make_integer_sequence;

template<int... V>
void g(integer_sequence<int,V...>)
{}

template<typename ...T>
struct c1
{
  static constexpr int value = 1;
  constexpr operator int() { return value; } 
};
template<typename T>
struct R
{
	using S = make_integer_sequence<int,c1<T>{}>;

	R() noexcept(noexcept(g(S())))
	{}
};
int main()
{
        R<int>();
}
