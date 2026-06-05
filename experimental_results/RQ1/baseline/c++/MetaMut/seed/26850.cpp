
template <class T>
concept bool C = false;

template <class>
constexpr bool trait = false;

template <C T>
constexpr bool trait<T> = true;

const bool& f() { return trait<int>; }

int main() {}

since it uses the base template, but not this one:

template <class T>
concept bool C = true;

template <class>
constexpr bool trait = false;

template <C T>
constexpr bool trait<T> = true;

const bool& f() { return trait<int>; }

int main() {}

