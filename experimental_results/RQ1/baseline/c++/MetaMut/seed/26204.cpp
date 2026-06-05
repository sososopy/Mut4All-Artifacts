

template <typename T>
struct DecltypeConstThis {

    T f() const { return T{}; }

    auto g() -> decltype(this->f()) { return this->f(); }
    auto h() const  ->  decltype(f()) { return f(); } // this should work the same as g() above (with implicit 'this')

};

struct Working {
    int f() const { return 0; }
    auto h() const -> decltype(f()) { return 0; }
};


int main() {

    Working w;
    w.h();

    DecltypeConstThis<int> d;
    d.g();
    d.h();

    return 0;
}
