
#include <string>
struct A { std::string s; };
struct B : virtual A {};

int main(int argc, char* argv[]) {
    B b1, b2;
    b2 = b1;
}
