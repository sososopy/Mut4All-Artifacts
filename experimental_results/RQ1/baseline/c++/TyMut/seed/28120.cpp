
template<class R=void, class... A>
struct bar {
    template<class B>
    bar(B& obj, R(B::*f)(A...)const=B::operator()){}
};
template<class R=void, class... A, class B> void foo(B& obj, R(B::*f)(A...)const=B::operator()){}
int main() {
    const auto f1 = [](){return 3;};
    //foo(f1);
    bar f8(f1);
}

