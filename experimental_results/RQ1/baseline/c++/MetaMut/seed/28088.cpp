
#include <initializer_list>
template <class> void b() {
	constexpr std::initializer_list<int> c{};
}
int main() { b<int>(); }
