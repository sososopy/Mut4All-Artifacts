

    struct Holder {
        void operator()() & { int read = data; }
        int data;
    };

    template <typename F = void()&>
    void test() {
        Holder h{42};
        F Holder::* fptr = &Holder::operator();
        (h.*fptr)();
    }

    int main() {
        test();
    }

