
struct Array  {
    static constexpr bool isArray() noexcept { return true; }
};

template <typename JSONVariant>
constexpr void buggyImpl(const JSONVariant& JSONValue) noexcept {
    constexpr bool t = JSONValue.isArray();
}

int main() {
    buggyImpl(Array{});
    return 0;
}

