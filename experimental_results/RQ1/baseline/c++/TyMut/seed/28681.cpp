
constexpr int i{0};
consteval const int& iref() { return i; }
[[maybe_unused]] constexpr const int* a{&iref()};
