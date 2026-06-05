
template <class Derived>
class iterator_facade
{
public:
    Derived& operator++()
    {
    }
    Derived operator--(int)
    {
    }
};

class IfSomethingIterator : public iterator_facade<IfSomethingIterator>
{
public:
    virtual ~IfSomethingIterator()
    {}
protected:
    virtual void DoIncrement() = 0;
};
