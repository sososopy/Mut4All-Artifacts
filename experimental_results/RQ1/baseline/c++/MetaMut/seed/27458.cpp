
int main() {
    int a[3]{};
    void const* b[3](a);  // ok

    void const* c[3]{};
    void const* d[3](c);  // error: array must be initialized with a brace-enclosed initializer
}
