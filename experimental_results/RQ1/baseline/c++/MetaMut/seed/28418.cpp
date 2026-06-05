
#include <iostream>
#include <memory>

struct ConstructMe {
    ConstructMe() {
        std::cout << "I have been constructed" << std::endl;
    }

    ~ConstructMe() {
        std::cout << "I have been destructed" << std::endl;
    }
};

struct Foo {
    Foo()
    : me{ std::make_unique< ConstructMe >() } {
        std::cout << "Foo" << std::endl;
    }

    struct {
        std::unique_ptr< ConstructMe > me[1];
    };
};

int main() {
        Foo f;
    return 0;
}

