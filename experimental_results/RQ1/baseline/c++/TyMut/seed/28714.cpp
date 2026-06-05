
struct Foo {};
Foo get_foo();

int main() {
    new Foo[1]{get_foo()};
}

