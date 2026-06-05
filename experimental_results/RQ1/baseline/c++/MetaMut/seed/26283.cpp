
typedef int F() const;

F f;

struct A
{
  friend F f;
};
