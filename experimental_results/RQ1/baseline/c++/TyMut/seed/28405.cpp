
struct foo {
    int x;
};

struct bar {
    foo f;
    friend bool operator==(const bar& a, const bar& b);
};

bool operator==(const bar& a, const bar& b) = default;

int main() {
    return bar{} == bar{};
}
