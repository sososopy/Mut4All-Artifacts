
template <class T>
class ext
{
  virtual int fox(int x) {
    x = _Cilk_spawn fox(0);
  }
};

void bar() {
  ext<int> ex;
}
