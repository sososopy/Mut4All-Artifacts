
struct Foo {
  long a;
  const long b;
};

void Fn(const Foo&);

void Bar() {
  Fn( {1} );
}
