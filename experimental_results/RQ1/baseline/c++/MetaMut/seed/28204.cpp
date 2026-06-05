
#include <initializer_list>

template<typename T>
void Task() {}

auto a = &Task<int>;
auto b = { &Task<int> };
std::initializer_list<void(*)()> c = { &Task<int> };
auto d = { static_cast<void(*)()>(&Task<int>) };

