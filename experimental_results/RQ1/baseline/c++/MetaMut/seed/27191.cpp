
class Foo {
public:
    void SetParent(Foo *f)
    {
        parent = f;
    }

    Foo *GetParent()
    {
        return parent;
    }

    Foo *parent = nullptr;
};

int main(int argc, char **argv)
{
    Foo f;
    Foo f2;
    Foo *f3 = f.getParent();
}


