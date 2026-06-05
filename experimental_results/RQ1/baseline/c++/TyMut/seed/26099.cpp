struct A
{
    static A data[1];
    static A *foo() { return data; }
};

int main(int argc, char **argv)
{
  A::foo();
}

