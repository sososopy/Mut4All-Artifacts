

//#include <iostream>
#include <utility>

template <typename... Funcs>
struct composer;

template <typename Func>
struct composer<Func> {
	auto operator()(Func&& f) {
		return f;
	}
};

template <typename Func, typename... Funcs>
struct composer<Func, Funcs...> {
	auto operator()(Func&& f, Funcs&&... fs) {
		composer<Funcs...> c;
		return [&] (auto v) {
			return f(composer<Funcs...>()(std::forward<Funcs>(fs)...)(v));
		};
	}
};

template <typename... Funcs>
auto compose(Funcs&&... fs) {
	return composer<Funcs...>()(std::forward<Funcs>(fs)...);
}


int main (int argc, char const* argv[]) {
	float v = 3.5f;
	auto func = [] (auto v) { return int(v-0.5); };
	auto t = compose([] (auto v) { return v >= 3; }, func)(v);
//	std::cout << std::boolalpha << t << "\n";
	auto f = compose([] (auto v) { return v > 3; }, func)(v);
//	std::cout << std::boolalpha << f << "\n";
}

