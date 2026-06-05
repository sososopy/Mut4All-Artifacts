

struct X {
    X() noexcept(noexcept(y+1)) { }
    int y;
};

int main() { }

