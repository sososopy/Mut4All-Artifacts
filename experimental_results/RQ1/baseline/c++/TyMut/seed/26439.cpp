
struct t
{
  int i, j;
};
t y;
static auto [i, j] = y;
int &r = i;
int s = i;
int *t = &i;
int main() {}
