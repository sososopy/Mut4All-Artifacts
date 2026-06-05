
template <class T> struct Func {
  static auto apply() {}
};

template <class T>
struct A {
    T x;
};

template <class T>
A(T) -> A<T>;

int main() {
    A a{Func<double>::apply};
}

