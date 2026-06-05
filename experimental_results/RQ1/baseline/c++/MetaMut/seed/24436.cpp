
struct timeval
{
  int tv_sec;
};
// needs to be a template
template<typename T> 
int test(int flag)
{
     struct timeval a, b;
     return ((a.tv_sec == b.tv_sec ? false : true) + flag);
     // needs to use `flag'/operator
}
