
static union {
  int yz;
};

struct ra {
  static int &dy;
};

int &ra::dy = static_cast<int &> (yz);

bool
bm ()
{
  return ra::dy == yz;
}
