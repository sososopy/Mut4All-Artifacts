
template <class, class> struct same {static constexpr bool value = false;};
template <class T> struct same<T, T> {static constexpr bool value = true;};

int main() {
    float x, &r = x;
    [=] { // x and r are not captured (appearance in a decltype operand is not an odr-use)
        static_assert(same<decltype(x), float>::value, "");
        static_assert(same<decltype((x)), float const&>::value, "");
        static_assert(same<decltype(r), float&>::value, "");
        static_assert(same<decltype((r)), float const&>::value, "");
    }();
}
