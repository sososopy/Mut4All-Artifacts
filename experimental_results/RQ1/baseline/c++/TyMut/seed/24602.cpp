
namespace b {
  enum type_t { warning };
}

struct d {
  d(b::type_t) { }
  int operator()() { return 0; }
};

int main()
{
  d(b::warning)() + 1;
}
