
void testNewUseAfterFree() {
  int *p = (int *)operator new(0);
  int j = *p;
}
