
template <typename T, typename U>
constexpr bool is_same_v = false;

template <typename T>
constexpr bool is_same_v<T, T> = true;

template <typename T>
auto make() -> T;

template <typename T>
void AssertMakeSame() {
   static_assert( is_same_v<T, T>, "Type should be the same as itself");
   static_assert( is_same_v<decltype(make<T>()), T>, "make should produce the same type");
};

int main() {
   AssertMakeSame<int>();
// AssertMakeSame<int const>();  // Should be an error
   AssertMakeSame<int *>();
}


