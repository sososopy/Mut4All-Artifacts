
extern "C" void *malloc(unsigned long);
class a {
public:
  enum c { Array };
};
class d {
public:
  static int e(int);
};
class f {
public:
  int g;
  void operator=(int) { d::e(g); }
};
template < typename, int, int > using h = f;
template < int i, int j > using k = h< int, i, j >;
template < int i, int j > using l = h< int, i, j >;
class m {
public:
  k< 0, 1 > is_object;
  k< 1, 1 > length;
};
class n {
public:
  m *o() { return (m *)this; }
};
class p {
public:
  enum {} alloc;
  n *header;
  p(a::c) {
    header = (n *)malloc(alloc);
    m b = *header->o();
    b.length = 0;
  }
};
void detach2() { p(a::Array); }
