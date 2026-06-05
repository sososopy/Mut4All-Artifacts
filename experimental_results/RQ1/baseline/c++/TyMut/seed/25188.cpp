
int a; 

struct S 
{
  int i:8;
} b;

int
fn1 ()
{
  return &fn1 ? b.i : a;
}
