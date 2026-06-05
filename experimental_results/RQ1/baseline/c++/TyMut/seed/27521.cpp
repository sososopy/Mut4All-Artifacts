
#include <stdio.h>
#include <utility>

struct I {
    I() { puts(__PRETTY_FUNCTION__); }
    I(I&) { puts(__PRETTY_FUNCTION__); }
    I(const I&) { puts(__PRETTY_FUNCTION__); }
    I(I&&) { puts(__PRETTY_FUNCTION__); }
    I(const I&&) { puts(__PRETTY_FUNCTION__); }

    void operator++() const {}
};

int main() {
    I i;
    auto one = [=]() { 
        return [=]() {
            ++i;
        };
    }();
    puts("-----");
    auto two = std::move(one);  // !!
}
