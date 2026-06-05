
struct A { char i, a[]; };

struct A a1 = { 3, "AB" };                  // wrong in 5, okay in 6
struct A a2 = (struct A){ 3, "AB" };        // wrong in 5, okay in 6

struct B1 {
    A a3;
    B1 (): a3 { 3, "AB" } { }               // wrong in 5 and 6
} b1;

struct B2 {
    A a4;
    B2 (): a4 ((struct A){ 3, "AB" }) { }   // wrong in 5 and 6
} b2;

int main ()
{
#define PA(x) \
    __builtin_printf ("%i, { %i, %i, %i }\n", \
                      x.i, x.a[0], x.a[1], x.a[2])

  PA (a1);
  PA (a2);
  PA (b1.a3);
  PA (b2.a4);
}
                      
