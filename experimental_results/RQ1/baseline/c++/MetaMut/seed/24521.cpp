
struct A { A (); ~A (); };
struct B { A b[4]; };
struct C { B c[5]; };
const C e = {};
