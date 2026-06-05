
struct A
{
private:
    static const int private_of_A = 5;
};

template <typename T>
struct B : public A
{
    int get()
    {
        return private_of_A;
    }
};

int main()
{
    B<int> b;
    return b.get();
}

