
class FooBar
{
  private:
    long long value_;

  public:
    FooBar() : value_( 0 ) {}

    bool isUnused() const
    { return value_ == 0; }
};

struct Stream
{
    template<class T>
    Stream& operator<<( T const& t )
    {
      return *this;
    }
};

void xxx( Stream& os )
{
  FooBar bar;
  long long foo = 0;
  if( !bar.isUnused() )
  {
    os << foo << foo << foo << foo << foo << foo << foo 
       << foo << foo << foo << foo << foo << foo << foo
       << foo << foo << foo << foo << foo << foo << foo 
       << foo << foo << foo << foo << foo << foo << foo;
  }
}
