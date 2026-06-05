
namespace a {
class b {
public:
  virtual ~b();
  virtual void c();
};
} // namespace a
class d {
protected:
  virtual ~d();
};
class f : d, a::b {
public:
  using a::b::c;
};
class g final : f {
  void e();
};
void g::e() {
  c()
};
