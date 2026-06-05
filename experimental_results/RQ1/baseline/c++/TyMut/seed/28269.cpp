
template <typename, auto = []{}> concept x = true;
struct S {
    // GCC error: template argument 2 is invalid
    operator x auto() { return 42; }
    // GCC error: wrong number of template arguments (0, should be at least 1)
    operator x<> auto() const { return 43; }
};

