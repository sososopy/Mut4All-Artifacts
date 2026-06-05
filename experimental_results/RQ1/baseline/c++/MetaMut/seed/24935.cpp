

void f (int) __attribute__ ((alloc_size (1)));
long g (int*) __attribute__ ((alloc_size (1)));

int foo (void)
{
  int i = 17;
  void *p = (void*)g (&i);
  return __builtin_object_size (p, 0);
}

;; Function foo (foo, funcdef_no=0, decl_uid=1799, cgraph_uid=0, symbol_order=0)

foo ()
{
  int i;

  <bb 2> [100.0%]:
  i = 17;
  g (&i);
  i ={v} {CLOBBER};
  return -1;

}
