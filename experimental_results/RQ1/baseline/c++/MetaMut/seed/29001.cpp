
#include <initializer_list>
struct S { S( std::initializer_list<int> = {} ) {} };
int main() { S{0}; }
