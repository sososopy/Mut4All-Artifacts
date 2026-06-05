
template <typename, typename>
struct is_same { static constexpr bool value = false; };

template <typename T>
struct is_same<T, T> { static constexpr bool value = true; };

template <typename T, typename U>
static constexpr bool is_same_v = is_same<T, U>::value;

using uintptr_t = unsigned long long;

template <int const* I>
struct Parameterized { int const* member; };

template <typename T>
auto create() {
    static constexpr int const I = 2;

    return Parameterized<&I>{ &I };
}

int main() {
    auto one = create<short>();
    auto two = create<int>();

    if (is_same_v<decltype(one), decltype(two)>) {
        return reinterpret_cast<uintptr_t>(one.member) == reinterpret_cast<uintptr_t>(two.member) ? 1 : 2;
    }

    return 0;
}

