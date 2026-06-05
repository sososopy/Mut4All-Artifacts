struct b {};
struct d : b {
        using t = b;
        using t::t;
};

