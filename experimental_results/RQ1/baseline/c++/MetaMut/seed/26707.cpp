
template <typename ...Anything>
auto variable_template = [] { return 1; }();

int main() {
    variable_template<>;
}
