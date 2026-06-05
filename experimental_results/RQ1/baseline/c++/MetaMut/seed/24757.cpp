// RUN: %clang_cc1 -std=c++2b -fsyntax-only -verify %s
// expected-no-diagnostics

struct S {
    void f(this S&);
    void f(this S&, int);
};
using f0_t = void (*)(S&);
constexpr f0_t f0 = &S::f;
using f1_t = void (*)(S&, int);
constexpr f1_t f1 = &S::f;
static_assert(f0 == f0); // ICE caused by this
static_assert(f0 != f1);
static_assert(f1 == f1);