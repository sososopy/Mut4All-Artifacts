
struct wrapper {
    int& ref;
    constexpr wrapper(int& ref) : ref(ref) {}
};

template <const wrapper& X>
void fun1() {}

template <wrapper X>
void fun2() {
    fun1<X>();
}

int main() {
    static int val = 22;
    fun2<val>();
}

