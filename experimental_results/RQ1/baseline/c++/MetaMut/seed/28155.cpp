

template <typename a> class b {
public:
  b(const a &);
};
struct {
  int *c;
} d;
void e() { b<const int *>(d.c); }
