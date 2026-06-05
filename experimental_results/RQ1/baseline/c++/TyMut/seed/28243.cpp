
class O
{
public:
    int f() {
        return 1;
    }
};

template<typename T>
class A
{
public:
    A();
    A(int a);

protected:
    O p = O();
};

template<typename T>
class B : virtual public A<T>
{
public:
    B() {
    }
    B(int a) {
    }

protected:
    using A<T>::p;
    unsigned int k = p.f();
};

template class B<int>;

int main()
{
    return 0;
}
