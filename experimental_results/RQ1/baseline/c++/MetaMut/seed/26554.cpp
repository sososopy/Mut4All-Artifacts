
template <typename T>
struct Span {
    T* data_;
    int len_;

    [[nodiscard]] constexpr auto operator[](int n) const noexcept -> T& { return data_[n]; }
    [[nodiscard]] constexpr auto front() const noexcept -> T& { return data_[0]; }
    [[nodiscard]] constexpr auto back() const noexcept -> T& { return data_[len_ - 1]; }
};

auto get() -> Span<int>;

auto f() -> int {
    int const& a = get().front(); // warning
    int const& b = get().back();  // warning
    int const& c = get()[0];      // warning

    return a + b + c;
}

