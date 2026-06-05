
void* operator new (__SIZE_TYPE__, void *p) { return p; }

struct S { char c [32]; } *p [2];

void f ()
{
  new (p [0]) S;
}
