
 #include <cstdlib>
 #include <new>
 struct S { ~S() { std::abort(); } };
 int main() {
     new (std::nothrow) S[1];
 }

