

    struct C {
        C(){}
        template<typename T> explicit operator T () const { return T(42); }
        operator int () const { return 1; }
        operator double () const { return 3.14; }
    };

    unsigned foo() {
        C x;
        unsigned y = x;
        return y;
    }

