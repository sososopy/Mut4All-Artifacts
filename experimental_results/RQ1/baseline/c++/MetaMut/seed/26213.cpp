
#include <initializer_list>

template <typename T>
void function(std::initializer_list<T>) {
}

int main() {
    function({"foo", "bar"});
}

This program fails to compile:

#include <initializer_list>

template <typename... T>
void function(std::initializer_list<T>...) {
}

int main() {
    function({"foo", "bar"});
}

