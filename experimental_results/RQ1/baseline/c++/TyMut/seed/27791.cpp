
class A   // uses CLASS
{
public:
  struct A *p;   // uses STRUCT
}
[[maybe_unused]];   // misleading warning refers to STRUCT
