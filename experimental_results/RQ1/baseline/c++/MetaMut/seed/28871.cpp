template<typename T>
void fn (T);

class C {
  friend void ::fn(int);
  friend void ::fn<>(double);
  friend void ::fn<double *>(double *);
};

