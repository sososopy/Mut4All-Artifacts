
template<typename T>
class A {
        public:
                typedef T D;
};

template<typename X>
class B : public A<X> {
        using typename B::D; // Triggers segfault and probably invalid C++
        //using typename A<X>::D; // Works fine and probably valid C++
        public:
                D echo(D x) {
                        return x;
                }
};

int main() {
        B<int> b;
}

