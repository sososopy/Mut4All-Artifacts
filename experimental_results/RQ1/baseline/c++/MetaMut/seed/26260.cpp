
extern double f1(double);
struct s1
{
  double v;
};
inline auto f1(const s1& p) { return s1 { f1(p.v) }; }
struct s2
{
  double v;
  auto operator-(const s2& r) const { return s2 { v - r.v }; }
};
struct s3
{
  double v;
  s1 operator/(const s3& r) const { return s1 { v / r.v }; }
};
void f2(s3 p1, s3 p2) { auto d1 = f1(p1 / p2); }
