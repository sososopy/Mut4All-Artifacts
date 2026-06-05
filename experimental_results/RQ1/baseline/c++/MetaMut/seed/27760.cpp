
void test() {
    []() __attribute__((noinline,cold)) {
        asm volatile("");
    }();
}

