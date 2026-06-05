
struct base
{
protected:
  using type = int;
  base(type, type) {}
};

struct derived: public base
{
public:
  using base::base;
  using base::type;
};

int main()
{
  derived::type i;
  derived b(i,i);
}
