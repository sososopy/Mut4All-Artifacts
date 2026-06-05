

$ cat ostream.cc 
template<typename _CharT>
class basic_ostream
{
public:
  basic_ostream<_CharT>
  operator<<(int& (*__pf) (int&));
};

basic_ostream<char> cout;

int main(){
  cout << reinterpret_cast<int>(0.0f);
}
