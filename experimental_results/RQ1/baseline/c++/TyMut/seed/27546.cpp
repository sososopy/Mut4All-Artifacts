
class IDestroyable
{
protected:
    virtual void  Destroy() = 0;

public:
    void operator delete (void * ptr)
    {
        if (ptr != nullptr) static_cast<IDestroyable *>(ptr)->Destroy();
    }
};

class IToto
{
public:
    virtual void  Toto() = 0;
};

class ITotoDestroyable : public IToto, public IDestroyable
{
public:
    void operator delete (void * ptr)
    {
        if (ptr != nullptr) delete static_cast<IDestroyable *>(static_cast<ITotoDestroyable *>(ptr));
    }
};

template <typename INTERFACE>
class DestroyableBase : public INTERFACE
{
protected:
    virtual void  Destroy()
    {
        ::delete this;
    }

public:
    virtual ~DestroyableBase()
    {
    }

    void operator delete (void * ptr)
    {
        ::operator delete (ptr);
    }
};

#include <iostream>

class TotoDestroyable : public DestroyableBase<ITotoDestroyable>
{
public:
    ~TotoDestroyable()
    {
        std::cout << "OK Destroyed !\n";
    }

    void  Toto()
    {
        std::cout << "Toto !\n";
    }
};

int main()
{
    ITotoDestroyable * foo = new TotoDestroyable();
    // Uncomment to workaround the crash
    // foo->Toto();
    delete foo;
}


