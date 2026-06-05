
template <class x>
class z : x
{
public:
  bool zz (int) { return false; }
  int f(bool z) { return zz ? : 0; }
};

class t
{
};

int
main()
{
  z<t> x;
  return x.f(0);
}

