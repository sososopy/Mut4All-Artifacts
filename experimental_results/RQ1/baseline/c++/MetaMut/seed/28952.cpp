
class cl1 {
  virtual void m();
};
class cl2 : public cl1 {
public:
  int g();
  int h();
  int i();
};
class cl3 {
  cl1 *p;
  int g();
  int h();
  int i();
};
int cl3::g() {
  if (!p)
    return 0;
  cl2 *x = dynamic_cast<cl2 *>(p);
  return x->g();
}
int cl3::h() {
  if (!p)
    return 0;
  return (dynamic_cast<cl2 *>(p))->h();
}
int cl3::i() {
  if (!p)
    return 0;
  return dynamic_cast<cl2 *>(p)->i();
}

