
template<class T>
auto foo() { return; }

extern template auto foo<int>();

int main() { foo<int>(); }
