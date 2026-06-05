
template<typename T>
struct test {
    auto foo() {
        return *this;
    }
};

int main(int argc, const char* argv[]) {
    test<int> t;

    return 0;
}
