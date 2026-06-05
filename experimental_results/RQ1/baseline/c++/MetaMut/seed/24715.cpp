
static void testFunc(int i1, int i2) {
    (void)i1;
    (void)i2;
}

template <int... Ints> void wrapper() {
    testFunc(Ints...);
}

int main(int, char *[]) {
    wrapper<1, 2>();	
    return 0;
}

