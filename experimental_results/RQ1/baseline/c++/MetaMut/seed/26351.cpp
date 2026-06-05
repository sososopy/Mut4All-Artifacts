
template<typename T>
struct MyTemplate {
private:
    int x;
public:
    MyTemplate() : x(0) {}
    friend void non_templ_friend(MyTemplate val, MyTemplate<void> weird) {
        // MyTemplate<T> is a friend of non_templ_friend(MyTemplate<T>, MyTempl<void>)
        val.x++;  // always works
        weird.x++;  // should only work when T=void
    }
};
int main() {
    non_templ_friend(MyTemplate<int>(), MyTemplate<void>());
}

