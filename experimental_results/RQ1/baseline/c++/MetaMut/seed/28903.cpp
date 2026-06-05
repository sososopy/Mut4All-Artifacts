
export module A;
export import :Foo;
export int baz();

export module A:Foo;
import :Internals;
export int foo() { return 2 * (bar() + 1); }

module A:Internals;
int bar();

module A;
import :Internals;
int bar() { return baz() - 10; }
int baz() { return 30; }

