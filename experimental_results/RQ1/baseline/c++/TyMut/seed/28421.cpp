
struct A { int i; int f(); };
typedef int A::*MemPtr;
typedef int (A::*MemFuncPtr)();
struct B { MemPtr a[3]; MemFuncPtr b[3]; };
template <B> struct X {};
template <typename T>
typedef X<B{{}, { MemFuncPtr{ nullptr }}}> XB;


struct A { int i; int f(); };
typedef int A::*MemPtr;
typedef int (A::*MemFuncPtr)();
struct B { MemPtr a[3]; MemFuncPtr b[3]; };
template <B> struct X {};
template <typename T>
typedef X<B{{}, { MemFuncPtr{ 0 }}}> XB;


struct A typedef *MemPtr;
typedef (A::*MemFuncPtr) ();
struct B { int *b; MemFuncPtr c; };
template <B<B{{}, MemFuncPtr {nullptr}};

