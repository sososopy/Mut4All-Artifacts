

template<typename T>
  void
  construct(T* p)
  { ::new(static_cast<void*>(p)) T; }

template<typename T>
void
f(const T* t)
{
  construct(t);
}

int main()
{
  int i[1];
  f(i);
}

