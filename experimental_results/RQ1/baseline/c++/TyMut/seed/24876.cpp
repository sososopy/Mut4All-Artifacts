struct S {
    S(int);
};

int main() {
    S s(0); // no warning
    S s2 = 0; // unused
    S s3{0}; // no warning
}