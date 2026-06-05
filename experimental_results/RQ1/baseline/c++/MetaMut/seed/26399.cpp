
struct foo {
    int x;
};

struct dummy {
    template<typename U>
    explicit dummy(U&&);
    dummy() = default;
    dummy& operator=(foo&&);
};

int main() {
    dummy bar;
    bar = {1};
}

