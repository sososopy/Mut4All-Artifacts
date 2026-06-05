
class Foo
{
public:
  Foo(int i) : _M_i(i) { }
  int bar() & { return _M_i /= 2; }
  int bar() const & { return _M_i; }
  int bar() && { return 2 * _M_i; }

private:
  int _M_i = 42;
};

int
main()
{
  Foo ph(333);
  ph.bar();

  const Foo ff(123);
  ff.bar();

  Foo(333).bar();
}
