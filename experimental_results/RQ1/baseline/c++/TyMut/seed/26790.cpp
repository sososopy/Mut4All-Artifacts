

struct B {};
struct S { explicit operator B(); };

int main() {
    B const &t(S{});
}

