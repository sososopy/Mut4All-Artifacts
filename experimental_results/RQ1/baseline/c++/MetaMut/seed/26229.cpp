
struct dummy {
    friend dummy& operator<<(...) {};
} my_dummy;

int main() {
    my_dummy << "ch";
}
