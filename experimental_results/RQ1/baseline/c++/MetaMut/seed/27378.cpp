
template<typename>
struct A {
    static auto self_reference = A{};
};

int main() {
    A<void>{};
}
