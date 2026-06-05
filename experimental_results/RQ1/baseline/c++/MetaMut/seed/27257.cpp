
struct Foo
{
  enum Kind { a };

private:
  Kind Kind;
};

enum Foo::Kind foo();
