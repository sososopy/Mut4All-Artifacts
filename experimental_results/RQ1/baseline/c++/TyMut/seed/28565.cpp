
struct Res { };
 
struct A {
    virtual Res &&foo() &&;
};
 
struct B : A {
    Res &foo() && override; // allowed to compile by gcc, not by clang and MSVC 
};

