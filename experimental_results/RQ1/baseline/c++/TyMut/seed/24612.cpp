
struct Base { virtual ~Base() noexcept; };
struct A { ~A(); }; // Should mean noexcept
struct Derived: Base { A a; };
