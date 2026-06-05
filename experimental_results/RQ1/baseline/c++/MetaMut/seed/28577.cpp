
struct X
{
   [[nodiscard]] X(int);
};

void t()
{
   X x(0);
}

