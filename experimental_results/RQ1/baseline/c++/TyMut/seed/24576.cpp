
struct foo {
    template<class T>
    operator const T & () { static T t {}; return t;}
};

int main() {
    int t((foo()));
}

