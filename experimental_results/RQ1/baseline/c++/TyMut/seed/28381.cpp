
struct K {};
template<typename Base> struct S : Base {
        S<Base>() {}
};
int main() { S<K> s; }

