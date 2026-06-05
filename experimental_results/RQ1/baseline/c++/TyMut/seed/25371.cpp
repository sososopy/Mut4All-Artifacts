
struct QVariant {
  ~QVariant();
};
struct A {
  QVariant toValue;
};
extern "C" {
void atan2(int, int);
float sqrtf(float);
}
int a, b;
void QDeclarativeParentAnimationtransition() {
  float c;
  A d;
  if (b)
    c = sqrtf(a);
  atan2(0, c);
}

