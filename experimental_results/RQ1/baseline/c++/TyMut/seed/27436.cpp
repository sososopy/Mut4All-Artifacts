

#include <vector>

struct Weirdo {
    // nonstandard copy constructor
    bool data{false}; // initializer here required to reproduce bug
    Weirdo() = default;
    Weirdo(const Weirdo& o) {
        // data should still be set to false via {} above and not being listed
        // original code had this in the assignment operator, and the copy constructor called the assignment operator.
        if (this != &o) { // pointer check required to reproduce bug
            data = o.data;
        }
    }
};

struct Normal {
    bool p{false};
    Normal() = default;
    Normal(Normal const& o) : p(o.p /* warning here for no good reason */) { }
};

struct Combo {
    Combo() : v(), q() {}
    void g();
    
    Weirdo v;
    Normal q;
};

struct Y {
    void f();
    
    std::vector<Combo> comboList;
};

void Y::f() {
    for (Combo combo : comboList) { /* implicit copy required to reproduce bug */
        combo.v.data && (combo.g(), false);
    }
}
