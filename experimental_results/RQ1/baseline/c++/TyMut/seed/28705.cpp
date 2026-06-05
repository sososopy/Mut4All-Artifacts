class base {
public:
  base(void) {};
  base(base &&) = delete;
};

class foo : public base {
};

template<typename C>
inline C &bar(void)
{
  static C c{};
  return c;
}

const foo &baz(void)
{
  return bar<const foo>();
}

