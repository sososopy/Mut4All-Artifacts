
struct X
{
  int b = 0;
  int c = ((this->b=1), 1);
};

constexpr X o = { };

static_assert(o.b == 1);

