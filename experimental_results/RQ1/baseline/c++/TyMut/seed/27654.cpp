
template <typename T>
inline constexpr char test_impl = 0;

template <typename T>
inline constexpr auto *id = &test_impl<T>;

int main() {
    static_assert(id<int> != id<float>, "should not be equal");
}
