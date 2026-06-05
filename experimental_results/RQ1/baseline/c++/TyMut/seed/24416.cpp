
#include <stdlib.h>

int foo(double d) { return static_cast<int>(d); }

template <typename T>
class Test
{
public:
        Test(int id, T const & value) : m_id(id), m_value(value) {}

        void change()
        {
                m_value = foo(m_id);
        }

        void change(int seed)
        {
                m_id = foo(m_value) * seed;
        }

        T value() const { return m_value; }
private:
        int m_id;
        T   m_value;
};

int foo(int n) { return n; }

int main()
{
    Test<int> t(10, 20);
    t.change();
    t.change(2);

    Test<unsigned long> tul(10, 30);
    tul.change();
    tul.change(3);

    return 0;
}

