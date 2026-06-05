

typedef struct {
  struct {
    unsigned d[4];
    template<typename T>
    unsigned operator[] (T i) const { return d[i]; }
  } c;
} A;

