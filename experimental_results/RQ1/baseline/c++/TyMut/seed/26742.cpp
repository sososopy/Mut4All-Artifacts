
#include <iostream>
using std::cout;
using std::ostream;

enum S1 { S1v0 = 0, S1v1= 10, S1v2, S1v3 };


class Q
{
public:
           Q()               : mySN(nextSN++), s1val(S1v0)      { cout << "+Q #" << mySN << " Default\n"; }
           Q(const Q  &src ) : mySN(nextSN++), s1val(src.s1val) { cout << "+Q #" << mySN << " Copy from #" << src.mySN << '\n'; }
                                                                                 
  explicit Q(S1 x)           : mySN(nextSN++), s1val(x)         { cout << "+Q #" << mySN << " from S1\n"; }

private:
  int mySN;
  S1 s1val;

  static int nextSN;

  friend ostream& operator<<( ostream &out, const Q &obj );

};

int Q::nextSN = 1;

ostream& operator<<( ostream &out, const Q &obj )
{
  out << "Q #" << obj.mySN << " == " << obj.s1val << '\n'; 
  return out;
}


int main( void )
{

  Q(S1v1);
  Q q2{S1v1};
  cout << '\n';

  cout << q2;

  return 0;
}
