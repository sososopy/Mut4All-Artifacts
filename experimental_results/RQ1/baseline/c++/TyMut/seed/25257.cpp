
#define PACKED __attribute__ ((packed))

#define TYPE_C short

typedef struct {
    TYPE_C c;
} PACKED test_struct;

class A
{
  const TYPE_C &c;
public:
  A (const TYPE_C & _c) :
    c(_c) {};
};

class B
{
public:
  B();
  A foo ();
private:
  test_struct * s;
};

A B::foo ()
{
  return A (s->c);
}


