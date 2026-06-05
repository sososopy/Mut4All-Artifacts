
void g(void);

void f(void) {
  __asm nop;
  __asm nop;
  __asm align 16;
  g();
}

