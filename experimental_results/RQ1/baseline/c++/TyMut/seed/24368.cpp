
struct v
{
  int (*f1) (int);
  int (*f2) ();
  int (*f3) (int);
  int (*f4) (int);
};

int func(int x) { return x; }
int fv() { return 23; }

struct v inst = {
  func,
  func,
  func,
  func
}; // Line 17

