
typedef __builtin_va_list a;
void b(int, a &);
struct c {
  operator a &();
  operator const a &();
};
void d() {
  c a;
  b(0, a);
}
