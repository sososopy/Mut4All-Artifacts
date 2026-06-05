
template<auto V>
void templ() {}

void foo() {
    constexpr auto * a = +[](){};
    templ<a>();
}

