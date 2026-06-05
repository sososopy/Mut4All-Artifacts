
template <typename T>
struct AbstractThing {
  virtual ~AbstractThing() { }
  virtual const T* method() const = 0;
};

template <typename T>
struct ConcreteThing : public AbstractThing<T> {
  virtual ~ConcreteThing() { }
  virtual const T* method() const { return 0; }
};

int main()
{
  struct Foo { };

  AbstractThing<Foo>* concrete = new ConcreteThing<Foo>();
  int rc = (concrete->method() == 0) ? 0 : 1;
  delete concrete;
  return rc;
}

