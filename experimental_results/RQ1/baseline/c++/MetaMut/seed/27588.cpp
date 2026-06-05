
void foo() noexcept {
    throw 42;
}

But not for

static void bar() {
    throw 42;
}
void foo() noexcept {
    bar();
}

