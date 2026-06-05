
union U
{
  int& i = 0;
};

void foo()
{
  U();
}
