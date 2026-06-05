
template<typename T>
concept bool C = true;
auto f() -> int& requires C<int>;

