
extern "C" void free(void *);
class Base
{
public:
        Base();
        void * operator new(unsigned long, const int &);
        void operator delete(void * ptr, const int &) { free(ptr); }
};
void f()
{
        new (0) Base;
}

