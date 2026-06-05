
int nothrow;
struct MaxAlignedAllocable {
  void *operator new[](unsigned long, int);
  void operator delete[](void *);
  long Resize_size;
  void Resize() { new (nothrow) MaxAlignedAllocable[Resize_size]; }
};
