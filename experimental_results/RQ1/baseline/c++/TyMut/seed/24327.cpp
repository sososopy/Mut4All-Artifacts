
namespace foo
{
  template < typename > class A
  {
    template < typename > friend class Outer;
  };
  class B:foo::A < int >
  {
  };
  template < typename > class Outer;
}

using foo::Outer;
