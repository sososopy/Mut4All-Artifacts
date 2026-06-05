
struct outer {
    struct inner {
         inner() :x(sizeof(y)) { }
        unsigned int x;
    };
    int y;
};

