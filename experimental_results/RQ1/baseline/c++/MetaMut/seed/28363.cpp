
template <class>
struct infinity{};

template <> struct infinity<__float128> { static constexpr __float128 value = __builtin_huge_valq(); };

int main() {
    auto x = infinity<__float128>::value;
}

