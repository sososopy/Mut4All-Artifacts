
struct Base {};
struct Derived : Base {};
Derived t() { Base b; return Derived(b); }


auto lam = [i=42]() { return i; };
struct Derived : decltype(lam) {
    bool is_derived() const { return true; }
};
Derived t() { return Derived(lam); }
