
struct A {
    __attribute__((noinline))
    A(int i) : data(i) {}
    A& operator+=(int i) { data += i; return *this; }
    int data;
};

int main() {
    A const& b = A(5) += 5;
    A c(6);
    return b.data + c.data; // 16 expected
}

