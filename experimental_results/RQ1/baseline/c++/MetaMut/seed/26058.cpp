
struct ABC {
  static double a;
  static double b;
  static double c;
  static double d;
  static double e;
  static double f;
};

double ABC::a = 1.0;
extern double ABC::b = 1.0;
static double ABC::c = 1.0;
__private_extern__ double ABC::d = 1.0;
auto double ABC::e = 1.0;
register double ABC::f = 1.0;
