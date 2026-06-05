
struct FunctionObject {
    void operator()() const { }
};

template <typename T>
constexpr FunctionObject f{};

// template FunctionObject f<int>;

int main() {
    f<int>();
}
