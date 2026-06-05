
template<typename T>
struct X {
    X(T _t) {}
};

template<typename T, typename U>
struct Y {
    Y(T _t, U _u) {}
};

int main() {
    X(1);
    new X(1);
    
    Y(1, 2);
    new Y(1, 2); // This line causes the compilation error.

    return 0;
}
