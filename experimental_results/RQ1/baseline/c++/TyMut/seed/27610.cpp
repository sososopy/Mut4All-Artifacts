
struct A {
    template<typename>
    void function_with_lambda() {
        [this](auto) {
            /*this->*/operator()();
        };
    }
	
    void operator()() {}
};

int main() {
    A{}.function_with_lambda<void>();
}
