
class a {
  virtual long b() const;
};
class c : a {
public:
  long b() const;
};
class d : c {
  long e();
};
long d::e() { b(); }

