
template <typename>
struct container
{
  struct elem {
    unsigned u;
  };
  struct incomplete {
    int x;
    elem array[];
  };
};

unsigned f (container<void>::incomplete* i)
{
  return i->array [0].u;
}
