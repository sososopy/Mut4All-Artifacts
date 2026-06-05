
struct A {
    A() 
    : a()       // erroneous value-initialization of reference
    , b(1i)     // erroneous complex conversion 
    , c('a')    // errors are incorrectly displayed at member c
    {}

    int &a;
    int b;
    char c;
};

