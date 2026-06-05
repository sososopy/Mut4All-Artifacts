
template<typename T> concept C = requires { typename T::value_type; };

template<typename Container>
    requires C<Container>
class Foo {
public:
    void func();
};

template<typename Container>
void Foo<Container>::func()
{}

