
template<typename> struct S { };

// [temp.deduct.guide]p3: Two deduction guide declarations in the same
// translation unit for the same class template shall not have equivalent
// parameter-declaration-clauses.
S() -> S<int>;
S() -> S<int>; // { dg-error "redeclared" }
S() -> S<float>; // { dg-error "ambiguating new declaration of" }

S(bool) -> S<int>;
explicit S(bool) -> S<int>; // { dg-error "redeclared" }


explicit S(char) -> S<int>;
S(char) -> S<int>; // { dg-error "redeclared" }

// OK: Use SFINAE.
template<typename T> S(T) -> S<typename T::foo>;
template<typename T> S(T) -> S<typename T::bar>;

// OK: Non-template wins.
S(int) -> S<int>;
template<typename T = int> S(int) -> S<char>;
