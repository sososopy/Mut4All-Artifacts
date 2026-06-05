
typedef struct xxx 
{
  unsigned long long a1;
  unsigned long long a2;
  unsigned long long a3;
  unsigned long long a4;
} xxx;

typedef struct Foo
{
  xxx foo;
} Foo;

extern xxx b;
Foo y;

void
foo (void)
{
  b = y.foo;
}
