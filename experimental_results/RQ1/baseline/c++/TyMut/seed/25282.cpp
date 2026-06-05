

    struct S {
        struct foo {};
        foo foo;
    };

    int main() {
        typename S::foo t;
        (void) t;
    }

