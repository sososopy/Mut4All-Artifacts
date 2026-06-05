
template<typename T, typename U>
concept Same = __is_same(T, U);

template<typename T>
void foo() { Same<T> auto x = 0; }

void bar() { foo<bool>(); }
