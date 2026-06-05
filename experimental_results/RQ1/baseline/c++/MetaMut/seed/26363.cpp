template<typename RAIter>
void
random_shuffle(RAIter, RAIter);

template<typename RAIter>
__attribute__ ((deprecated))
void
random_shuffle(RAIter, RAIter) { }

int main()
{
  int* p = 0;
  random_shuffle(p, p);
}

