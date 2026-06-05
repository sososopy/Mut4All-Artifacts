
struct A {
        explicit operator int() const;
};
explicit inline A::operator int() const { return 1; }

