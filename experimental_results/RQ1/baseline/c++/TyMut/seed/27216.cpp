
#include <concepts>
#include <type_traits>

struct WeirdBase {};
struct WeirdSub: WeirdBase {};

template <typename Foo, std::derived_from<WeirdBase> WeirdSub1>
auto
make_lambda()
{
	return [](std::derived_from<WeirdSub1> auto&&){};
}

auto
main() -> int
{
	auto const lambda = make_lambda<int, WeirdSub>();
	lambda( WeirdSub{} );

	return 0;
}
