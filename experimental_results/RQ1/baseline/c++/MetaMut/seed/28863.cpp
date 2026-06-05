

#pragma omp declare target
  int data[]={5};
#pragma omp end declare target

struct my { inline const int& operator[](const int idx) const { return data[idx]; } };

int main()
{
  int i;
  struct my cs;
  #pragma omp target map(to:i)
    i = cs[0];
  return i;
}
