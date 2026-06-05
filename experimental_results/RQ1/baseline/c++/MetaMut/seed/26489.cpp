

int foo(int *x, int *y) {
    __builtin_assume_separate_storage(x, y);
    *x = 0;
    *y = 1;
    // The optimizer may optimize this to return 0 without reloading from *x.
    return *x;
}

