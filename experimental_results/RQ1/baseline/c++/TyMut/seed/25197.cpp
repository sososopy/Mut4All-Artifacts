
class CentralFreeList {
 public:
  CentralFreeList() {}
};

template <int size>
class CentralFreeListPaddedTo : public CentralFreeList {
 private:
  char pad_[64 - size];
};

class CentralFreeListPadded
    : public CentralFreeListPaddedTo<sizeof(CentralFreeList) % 64> {};

class Static {
 public:
  static CentralFreeListPadded central_cache_[50];
};

