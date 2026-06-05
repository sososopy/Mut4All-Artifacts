
struct Printy {
    Printy(const char *name) : name(name) {
        std::cout << "Printy(" << name << ")\n";
    }
    ~Printy() { std::cout << "~Printy(" << name << ")\n"; }
    const char *name;
};

struct Awaiter : std::suspend_always {
    Printy await_resume() { return Printy(""); }
};

void foo(std::vector<Printy>) {}

coroutine foo() {
    foo({Printy("b"),
         co_await Awaiter{}});
    co_return;
}
