
void foo() {
  goto end;
  end:
#ifdef BAR
  ...
#endif
}
