struct S { S(int) {} S() = delete; };
int main() { new S[5] { 0, 1, 2, 3, 4 }; }

