
#include <vector>
struct S {};
struct Options {
    int opt{};
};
void A(std::vector<S>) {}
void A(Options) {}
int main() { A({.opt = 1}); }
