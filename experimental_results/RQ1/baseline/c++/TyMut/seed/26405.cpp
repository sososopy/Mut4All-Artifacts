

struct X {
    ~X() {}
};

struct Y {
    ~Y() noexcept(false) {}
};

X foo() {
    try {
        return {};
    } catch (...) {
        Y{};
        return {};
    }

    try {
    } catch (...) {
    }
}

