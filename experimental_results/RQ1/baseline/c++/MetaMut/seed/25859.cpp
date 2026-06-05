
class Foo
{
    virtual void Hello() {}

    float f;     /* 4 bytes */
};

class Bar
{
    virtual void Hello() {}

    float f;     /* 4 bytes */
    double d;    /* 8 bytes */
};

class EmptyBase
{
protected:
    virtual ~EmptyBase() {}
};

class SmallBar : public EmptyBase
{
    virtual void Hello() {}

    float f;     /* 4 bytes */
    double d;    /* 8 bytes */
};

int main() {
   return sizeof(Foo) + sizeof(Bar) + sizeof(SmallBar);
}

