
struct Descriptor {};

template <Descriptor & D>
struct foo
{
  void size ();
};

Descriptor g_descriptor = {};

template<> void foo<g_descriptor>::size()
{
}
