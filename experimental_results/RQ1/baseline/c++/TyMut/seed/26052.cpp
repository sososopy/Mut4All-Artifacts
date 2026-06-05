
#pragma weak foo
struct S {  void foo(); };
void S::foo() {}
#pragma weak zed
namespace bar {  void zed() {} }
#pragma weak bah
void bah() {}
#pragma weak baz
extern "C" void baz() {}

