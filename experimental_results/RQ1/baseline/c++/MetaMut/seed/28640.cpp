// clang compiles without errors.
#include <tuple>
#include <type_traits>
#include <iostream>
using namespace std;

template <typename T>
constexpr bool check_visitor = []<size_t ...I>(index_sequence<I...>){
	    return ((requires (tuple_element_t<I, T> fun)
					{{fun(3)}->same_as<void>;}) && ...);
}(make_index_sequence<tuple_size_v<T>>{});

int main() {
	auto f1 = [](int) {};
	auto f2 = [](int) {};
	static_assert(check_visitor<tuple<decltype(f1), decltype(f2)>>);
}

