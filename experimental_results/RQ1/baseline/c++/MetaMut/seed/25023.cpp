

class GeneralIO {
protected:
  int util_;
  bool bin_;
  ~GeneralIO();
};
class A : GeneralIO {
  A(const GeneralIO &);
};
A::A(const GeneralIO &p1) : GeneralIO(p1) {}

