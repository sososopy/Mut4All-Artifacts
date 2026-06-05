
struct ThreadWrapper {
  virtual ~ThreadWrapper() = 0;
};
void foo() {
  ThreadWrapper *y;
  y->~ThreadWrapper();
}

