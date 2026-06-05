
#include <concepts>

template<typename T>
struct Struct {
static void func();
};

template<std::same_as<int> T>
struct Struct<T> {
static void func() {}
};

template<std::same_as<double> T>
struct Struct<T> {
static void func() {}
};

template struct Struct<double>;
template struct Struct<int>;

