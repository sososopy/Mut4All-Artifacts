
template<class T> 
T* calloc_or_die()
{
  void* data = calloc(1, sizeof(T));
  if (data == nullptr) throw std::exception();
  return (T*)data;
}
