//$ cat x.cpp
struct __attribute__((dllexport)) foo {
	static constexpr int i = 3;
};
constexpr int foo::i;
//$ cat y.cpp
struct __attribute__((dllimport)) foo {
	static constexpr int i = 3;
};
int main() {
	volatile auto v = &foo::i;
}
