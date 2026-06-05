
template<typename> concept C0 = true;
template<typename> concept C1 = true;

template<typename T>
struct X {
    ~X() requires C0<T> {}
    ~X() requires C1<T> {}
};

int main () {
    X<int> x;
}
