
struct D
{
  const int &x;
  const int &y;
};
int Create (const D &);

void fn1 ()
{
  Create ( { {}, {} });
}
