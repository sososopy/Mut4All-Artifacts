
#include <type_traits>

template <typename T, typename = void> struct status : std::false_type{};

template <typename T> struct status<T, decltype(T::member, void())> : std::true_type {};

struct s1{int member;};
struct s2{int _member;};

int main(){
	static_assert(status<s1>::value, "has member");
	static_assert(!status<s2>::value, "has no member");
}
