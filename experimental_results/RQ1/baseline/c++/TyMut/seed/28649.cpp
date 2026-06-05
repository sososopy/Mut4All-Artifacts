
struct S6 {
  int aa[2];
};

int main(int argc, char **argv) {
  S6 s6[1];

#pragma omp target update from(s6[0].aa[0:1])

  return 0;
}

