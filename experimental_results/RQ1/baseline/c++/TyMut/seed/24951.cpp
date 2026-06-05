
template<typename> class fl {};
template<typename = void, template<class...> class = fl>
struct S {};
template<typename... T>
void f(S<T...> ) {}
void lol() {
    S<> s;
    f(s);
}
