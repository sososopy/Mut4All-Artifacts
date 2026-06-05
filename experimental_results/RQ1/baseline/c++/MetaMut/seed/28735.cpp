
template <auto = 0>
auto g() requires ([] { return 0; } ());

int main() { g(); }

