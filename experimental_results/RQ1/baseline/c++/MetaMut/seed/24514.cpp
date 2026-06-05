
template <class T> class nsRefPtr {
  T *mRawPtr;

public:
  typedef T element_type;
  ~nsRefPtr() { mRawPtr->Release; }
};

class nsSlots {
public:
  virtual ~nsSlots();
};
class A;
class B : nsSlots {
  nsRefPtr<A> mContainingShadow;
};

