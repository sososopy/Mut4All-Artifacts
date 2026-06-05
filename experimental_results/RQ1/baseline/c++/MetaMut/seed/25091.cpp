
class QBasicAtomicPointer {
public:
  typedef int *Type;
  constexpr QBasicAtomicPointer(Type);
};
template <typename T> class QAtomicPointer : QBasicAtomicPointer {
public:
  constexpr QAtomicPointer(T *value = 0) : QBasicAtomicPointer(value) {}
};

class JobQueue {
  void m_fn1();
  QAtomicPointer<int> m_items[4];
} MixerWorkerThreadglobalJobQueue;
void JobQueue::m_fn1() {
  int *_job;
  m_items[1] = _job;
}

