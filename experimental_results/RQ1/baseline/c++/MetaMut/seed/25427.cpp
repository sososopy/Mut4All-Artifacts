
struct Foo { virtual void frob(int i)=0; };
void frobN(Foo* f, int n) {
   for (int i=0; i < n; i++)
      f->frob(i);
}
