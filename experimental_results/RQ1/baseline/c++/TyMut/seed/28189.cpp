
struct X {};

using MemFn = int (X::*)();

template <class T>
struct Wrap {
    T data;

    Wrap() : data() {}
};

int main() {
    Wrap<MemFn const> x;
}

