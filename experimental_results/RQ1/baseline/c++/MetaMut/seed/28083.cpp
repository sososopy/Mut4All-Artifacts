
class a {
public:
  ~a();
};
long b;
void c() {
  a *d = new a[b];
  delete[] d;
}
