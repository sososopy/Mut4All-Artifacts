

void meow() {
    void purr();
    void (&f)() = purr;
    [f]{};
}

