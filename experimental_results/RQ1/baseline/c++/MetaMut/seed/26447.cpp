
template <typename T>
class Outer
{
public:
  struct Inner {
    friend bool operator==(const Inner& a, const Inner& b) {
      return a.outer_.private_member_ == b.outer_.private_member_;
    }
   
  Inner(Outer<T> & o):outer_{o} 
  {

  }
   private:
    Outer<T>& outer_;
    
  };
  friend struct Iterator;
 //friend bool operator==(const Inner& a, const Inner& b);
  private:
  int private_member_;
};

bool test(Outer<int>::Inner A, Outer<int>::Inner B)
{
    return A == B;
}

int main()
{
    Outer<int> o; 
    Outer<int>::Inner i(o); 
    Outer<int>::Inner j(o);
    if(i==j){}  
}
