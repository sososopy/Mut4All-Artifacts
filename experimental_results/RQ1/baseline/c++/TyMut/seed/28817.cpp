
struct TestStruct {
  TestStruct() {
#pragma omp for ordered(2)
    for (int j = 0; j < 1; j++)
      for (int i = 0; i < 1; i++)
      {
#pragma omp ordered depend(source)
        ;
      }
  }
};

