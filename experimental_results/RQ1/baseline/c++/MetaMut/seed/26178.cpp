
struct Foo
{
    void call()
    {}
};

template<class MEMSIG, MEMSIG MEMFUNC>
struct Wrap
{
    inline static void call( Foo cc )
    {
      (cc.*MEMFUNC)();     // <- warning here
    }
};

void bar()
{
  Wrap<void (Foo::*)(), &Foo::call>::call( Foo() );
}
