
class A {
class B { };
friend class X;
};
class X : A::B { // ill-formed: A::B cannot be accessed
// in the base-clause for X
A::B mx; // OK: A::B used to declare member of X
class Y : A::B { // OK: A::B used to declare member of X
A::B my; // ill-formed: A::B cannot be accessed
// to declare members of nested class of X
};
};

