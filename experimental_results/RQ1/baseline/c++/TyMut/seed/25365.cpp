struct B {
    B() {}
    int x;
    int a : 6;
    int b : 6;
    int c : 6;
};

struct C : B {
    char d;
};

C c;

int main()
{
  c.c = 1;
  c.d = 2;
}

