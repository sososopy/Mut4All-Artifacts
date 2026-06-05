struct QString
{
  QString toLower() const&;
  QString toLower() &&;
};

struct QCoreApplication
{
  static QCoreApplication* self;
  static QCoreApplication *instance() { return self; }
  static QString applicationName();
};

template <typename HandlerType>
class WorkerPool  {
 public:
  WorkerPool();
};

template <typename HandlerType>
WorkerPool<HandlerType>::WorkerPool()
{
  QCoreApplication::instance()
    ->applicationName().toLower();
}

WorkerPool<int> w;



