
template <typename T>
struct DecltypeConstThis {

    void f() {}
    T f() const { return T{}; }

    auto g() -> decltype(this->f()) { return f(); }
    auto g() const  ->  decltype(this->f()) { return f(); } // apparently 'this' is not const within the decltype() here

};

int main() {
    DecltypeConstThis<int> d;
    const DecltypeConstThis<int> &cd = d;

    d.g();
    cd.g(); // fails
    return 0;
}

