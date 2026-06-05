
struct base {
  template <typename type>
    requires false
  base(type);

  template <typename type>
    requires true
  base(type);
};

struct derived : base {
  using base::base;
};

int main() { derived{'G'}; }
