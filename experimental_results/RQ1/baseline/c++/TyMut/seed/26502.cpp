
#include <variant>

struct S {
	constexpr S(int i_) : i(i_) {}
	S(S const &) = delete;
	S & operator=(S const &) = delete;
	S(S &&) = delete;
	S & operator=(S &&) = delete;
	int i;
};

struct A {
	S s{0};
};

using V = std::variant<A>;

consteval auto f(V const & v) {
	auto const & s = std::visit([](auto const & v) -> S const & { return v.s; }, v);
	return s.i;
}

int main() {
	constexpr V a{std::in_place_type<A>};
	constexpr auto i = f(a);
	return i;
}
