
template<class T>
class simple_pointer {
private:
    T *ptr{};
public:
    constexpr simple_pointer() noexcept = default;
    constexpr simple_pointer(T *p) noexcept : ptr{p} {}
    simple_pointer(const simple_pointer &) = delete;
    constexpr simple_pointer(simple_pointer &&p) noexcept : ptr{p.ptr} {}
    constexpr ~simple_pointer() noexcept { delete ptr; }

    void operator=(const simple_pointer &) = delete;
    constexpr simple_pointer &operator=(simple_pointer &&p) noexcept {
        delete ptr;
        ptr = p.ptr;
        p.ptr = nullptr;
        return *this;
    }

    constexpr T &operator*() const noexcept { return *ptr; }
    constexpr T *operator->() const noexcept { return ptr; }
};

template<class T>
constexpr simple_pointer<T> make_simple() {
    return simple_pointer<T>(new T());
}


struct S {
    int x{42};

    // Removing this virtual...
    constexpr virtual ~S() noexcept = default;
};


consteval int run() {
    auto p = make_simple<S>();
    return p->x;
}

int main() {
    return run();
}

