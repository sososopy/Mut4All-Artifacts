
template <short, char> struct A;

int foo(void *);
template <short x> int foo(A<x, x> *ap)
   // neither clang++ nor g++ bother to look at the second instance of x
   // for deduction
{ return noGood(ap); }

int bar(void *);
template <char y> int bar(A<y, y> *ap)
   // okay; clang++ and g++ both realize that deducing from the first instance
   // of y is no good
{ return noGood(ap); }

int zip(void *, void *);
template <short x, char y> int zip(A<x, y> *ap, A<y, x> *)
   // clang++ does not bother to look at the second function argument
   // for deduction, g++ does
{ return noGood(ap); }

A<0, 0> *ap = 0;

int a = foo(ap);  // clang++ and g++ both fail to fail the argument deduction
int b = bar(ap);  // clang++ and g++ both successfully fail the deduction
int c = zip(ap, ap);  //
   // clang++ fails to fail and g++ successfully fails the argument deduction

// For all three:
// - MSVC 17.00.51025 fails to fail the argument deduction
// - ICC 13.0.1 20121010 and IBM XL C/C++ for AIX 12.1.0.3 work fine

