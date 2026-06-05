
void foo() {
    register const int r4 asm("r4") = 0x1000;
    asm volatile("swi #1" : : "r"(r4));
}

void bar() {
    register int r4 asm("r4") = 0x1000;
    asm volatile("swi #1" : : "r"(r4));
}
