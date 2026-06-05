
extern "C" {
class Foo {
  int val;
  friend int bar(Foo &);
};
}

extern "C" int bar(Foo &f) {
  return f.val;
}

