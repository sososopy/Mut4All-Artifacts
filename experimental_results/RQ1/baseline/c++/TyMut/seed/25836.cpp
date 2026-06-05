
struct S {
	static void foo() {}
};

template<typename... Args>
void run() {
	(void) (Args::foo()...);
}

int main() {
	run<S, S>();
}
