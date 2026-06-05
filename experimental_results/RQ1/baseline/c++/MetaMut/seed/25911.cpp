
    template <void (*F)(int)>
    class B {
        decltype(F) v;
    };

    void foo(int) {}

    int main() {
        B<foo> o;
    }

