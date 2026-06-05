

typedef int my_int;

template<typename T>
struct X {
    enum {value = 1};
};

template<typename T>
void f(const my_int(&)[X<T>::value]);

int main() {
    const my_int a[1] = {};
    f<void>(a);
}
