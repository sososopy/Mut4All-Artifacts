
enum class eclass { zero };

template<typename a>
using eclass_alias = eclass;

template<typename T0>
eclass_alias<T0> foo(T0 f) { return eclass_alias<T0>::zero; }

int main() {}
