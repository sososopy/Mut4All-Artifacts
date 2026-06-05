
#include <future>

class QThreadCreateThread
{
public:
    explicit QThreadCreateThread(std::future<void> &&future)
        : m_future(std::move(future))
    {
    }

private:
    virtual void run()
    {
        m_future.get();
    }

    std::future<void> m_future;
};

// QThread *QThread::createThreadImpl(std::future<void> &&future)
QThreadCreateThread *createThreadImpl(std::future<void> &&future)
{
    return new QThreadCreateThread(std::move(future));
}

