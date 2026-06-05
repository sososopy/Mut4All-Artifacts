
template<class Iterator>
struct X{
  Iterator p;
  X() : p(Iterator()){}
};

static_assert(__is_trivially_constructible(X<int*>), "...");

int main(){}
