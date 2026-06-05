
struct base1
{
  virtual ~base1();
};
  
struct base2
{
  virtual ~base2();
};
    
class base3 :	public base2, public virtual base1 {};

class istream_tie :
  public virtual base1,
  public virtual base3
{
};  

void foo()
{
  istream_tie in;
}
