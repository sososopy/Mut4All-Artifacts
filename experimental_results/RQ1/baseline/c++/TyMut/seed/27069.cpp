
void foo () {
    int x;
    {
        struct { int  : sizeof x; } b0; (void)&b0;
        struct { int x: sizeof x; } b1; (void)&b1;
        struct { int y: sizeof y; } b2; (void)&b2;
    }
}
