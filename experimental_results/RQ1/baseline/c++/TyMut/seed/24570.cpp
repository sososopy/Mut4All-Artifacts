
constexpr const int A = 42;
const int &B = A;
static_assert(&A == &B, "Bug");

int main() {return 0;}
