void g();

template<typename T>
struct Capture;

template<typename T>
struct Capture<T*>  {
    friend void g() {
        &T::a;
    }
};

struct X;

void f() {
    union {
        int a = sizeof(Capture<decltype(this)>);
    };
    g();
}